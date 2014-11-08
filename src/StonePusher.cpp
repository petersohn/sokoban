#include "StonePusher.hpp"
#include "Array.hpp"
#include "Status/Status.hpp"
#include "Status/State.hpp"
#include "VisitedStateInfo.hpp"
#include "Status/floodFill.hpp"
#include "intersect.hpp"
#include "FieldType.hpp"
#include <iostream>
#include <memory>

class InternalPusher {
public:
	typedef std::deque<std::pair<State, VisitedStateInfo> > PushListType;
private:
	Node::Ptr node_;
	const HeurCalculator& calculator_;
	NodeFactory& nodeFactory_;
	bool pushStone(const Status& status, Point p);
	bool pushStoneIter(const Status& status, Point p, Point d);
public:
	InternalPusher(const HeurCalculator& calculator, NodeFactory& nodeFactory):
		calculator_(calculator),
		nodeFactory_(nodeFactory)
	{}
	Node::Ptr pushStones(Status status, Node::Ptr base);
};

Node::Ptr InternalPusher::pushStones(Status status, Node::Ptr base) {
	node_ = std::move(base);
	Array<bool> destReachable(status.width(), status.height(), false);
	bool touched;
	bool touched2 = false;

	do {
		touched = false;
		Status::BorderType destBorder;
		floodFill(status, status.table().destination(), destReachable, destBorder);
		Status::BorderType border = intersect(status.border(), destBorder);
		for (Status::BorderType::const_iterator it = border.begin();
				it != border.end(); ++it) {
			if (pushStone(status, *it)) {
				status.set(*node_);
				touched = true;
			}
		}
		if (touched)
			touched2 = true;
	} while (touched);
	return touched2 ? node_ : Node::Ptr();
}

bool InternalPusher::pushStone(const Status& status, Point p) {
	if (p == status.table().destination())
		return true;
	if (pushStoneIter(status, p, Point::p10))
		return true;
	if (pushStoneIter(status, p, Point::pm10))
		return true;
	if (pushStoneIter(status, p, Point::p01))
		return true;
	if (pushStoneIter(status, p, Point::p0m1))
		return true;
	return false;
}

bool InternalPusher::pushStoneIter(const Status& status, Point p, Point d) {
	Point pd = p+d;
	Point pmd = p-d;
	if (status.value(pmd) != FieldType::floor || status.value(pd) != FieldType::floor || !status.reachable(pmd))
		return false;
	Status newStatus(status);
	if (newStatus.currentPos() == pd)
		newStatus.shiftCurrentPos();
	int heur1 = calculator_.calculateStone(newStatus, p);
	newStatus.currentPos(p);
	int heur2 = calculator_.calculateStone(newStatus, pd);
	if ((heur2 < 0 && (pd != newStatus.table().destination())) || heur2 >= heur1)
	{
		return false;
	}
	if (!newStatus.moveStone(p, pd))
	{
		std::cerr << "Whoopsie doopsie!";
		return false; // should never happen
	}
	node_ = nodeFactory_.createNode(newStatus, MoveDescriptor(p, d), node_);
	if (pushStone(newStatus, pd))
		return true;
	node_ = node_->ancestor();
	return false;
}


StonePusher::StonePusher(Expander::Ptr expander,
		VisitedStateHolder::Ptr visitedStates,
		HeurCalculator::Ptr calculator, NodeFactory::Ptr nodeFactory):
		expander_(std::move(expander)),
		visitedStates_(visitedStates),
		calculator_(std::move(calculator)),
		nodeFactory_(std::move(nodeFactory))

{
	assert(calculator_ != NULL);
}

void StonePusher::expand(const Status& status, std::shared_ptr<Node> base,
		NodePusher& queue, Dumper::Ptr dumper)
{
	InternalPusher sp(*calculator_, *nodeFactory_);
	Node::Ptr node = sp.pushStones(status, base);

	if (!node) {
		expander_->expand(status, std::move(base), queue, std::move(dumper));
		return;
	}

	Status newStatus(status.table(), *node);
	VisitedStateInput vsi(newStatus, node->costFgv());
	if (visitedStates_ && !visitedStates_->checkAndPush(vsi)) {
		if (dumper) {
			dumper->reject(node, "already visited");
		}
		return;
	}

	if (dumper) {
		if (base) {
			dumper->startPushing(base);
		}

		std::deque<Node::Ptr> path = pathToBase(node, base);
		for (const auto& node: path) {
			dumper->push(node);
		}
	}

	if (node->heur() == 0) {
		queue.push(node);
		return;
	}

	expander_->expand(newStatus, std::move(node), queue, std::move(dumper));
}


