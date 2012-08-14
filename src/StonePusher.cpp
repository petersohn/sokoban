#include "StonePusher.h"
#include "Array.h"
#include "Status/Status.h"
#include "Status/State.h"
#include "VisitedStateInfo.h"
#include "Status/StatusUtils.h"
#include <iostream>
#include <memory>

class InternalPusher {
public:
	typedef std::deque<std::pair<State, VisitedStateInfo> > PushListType;
private:
	Node::Ptr node_;
	HeurCalculator::Ptr calculator_;
	NodeFactory::Ptr nodeFactory_;
	bool pushStone(const Status& status, const Point &p);
	bool pushStoneIter(const Status& status, const Point &p, const Point &d);
public:
	InternalPusher(HeurCalculator::Ptr calculator, NodeFactory::Ptr nodeFactory):
		calculator_(calculator),
		nodeFactory_(nodeFactory)
	{}
	Node::Ptr pushStones(Status status, Node::Ptr base);
};

Node::Ptr InternalPusher::pushStones(Status status, Node::Ptr base) {
	node_ = base;
	Array<bool> destReachable(status.width(), status.height(), false);
	bool touched;
	bool touched2 = false;

	do {
		touched = false;
		Status::BorderType destBorder;
		floodFill(status, status.table().destination(), destReachable, &destBorder);
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

bool InternalPusher::pushStone(const Status& status, const Point &p) {
	if (p == status.table().destination())
		return true;
//	Point currentTmp(status.currentPos());
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

bool InternalPusher::pushStoneIter(const Status& status, const Point &p, const Point &d) {
	Point pd = p+d;
	Point pmd = p-d;
	if (status.value(pmd) != ftFloor || status.value(pd) != ftFloor || !status.reachable(pmd))
		return false;
	Status newStatus(status);
	if (newStatus.currentPos() == pd)
		newStatus.shiftCurrentPos();
	int heur1 = calculator_->calculateStone(newStatus, p);
	newStatus.currentPos(p);
	int heur2 = calculator_->calculateStone(newStatus, pd);
	if ((heur2 < 0 && (pd != newStatus.table().destination())) || heur2 >= heur1)
	{
		return false;
	}
	if (!newStatus.moveStone(p, pd))
	{
		std::cerr << "Whoopsie doopsie!";
		return false; // should never happen
	}
	node_ = nodeFactory_->createNode(newStatus, MoveDescriptor(p, d), node_);
	if (pushStone(newStatus, pd))
		return true;
	node_ = node_->ancestor();
	return false;
}


StonePusher::StonePusher(VisitedStateHolder::Ptr visitedStates,
		HeurCalculator::Ptr calculator, NodeFactory::Ptr nodeFactory):
		visitedStates_(visitedStates),
		calculator_(calculator),
		nodeFactory_(nodeFactory)

{
	assert(calculator != NULL);
}

bool StonePusher::expand(const Status &status, std::shared_ptr<Node> base,
		NodePusher& queue, Dumper::Ptr dumper)
{
	InternalPusher sp(calculator_, nodeFactory_);
	Node::Ptr node = sp.pushStones(status, base);
	if (node.get() == NULL)
		return false;
	queue.push(node);
	Status st(status.tablePtr());
	std::deque<Node::Ptr> path = pathToBase(node, base);
	for (std::deque<Node::Ptr>::iterator it = path.begin();
			it != path.end(); ++it) {
		st.set(**it);
		if (visitedStates_) {
			visitedStates_->checkAndPush(std::pair<const Status&, int>(st, (*it)->costFgv()));
		}
		if (dumper) {
			dumper->push(*it);
		}
	}
	return true;
}


