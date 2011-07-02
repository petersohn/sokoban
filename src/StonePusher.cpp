#include "StonePusher.h"
#include "Array.h"
#include "Status.h"
#include "State.h"
#include "VisitedStateInfo.h"
#include <iostream>
#include <memory>

class InternalPusher {
public:
	typedef std::deque<std::pair<State, VisitedStateInfo> > PushListType;
private:
	Node::Ptr node_;
	std::auto_ptr<Status> status_;
	HeurCalculator::Ptr calculator_;
	bool pushStone(const Point &p);
	bool pushStoneIter(const Point &p, const Point &d);
public:
	InternalPusher(HeurCalculator::Ptr calculator):
		calculator_(calculator) {}
	Node::Ptr pushStones(const Status &st, Node::Ptr base);
};

Node::Ptr InternalPusher::pushStones(const Status &st, Node::Ptr base) {
	node_ = base;
	status_.reset(new Status(st));
	Array<bool> destReachable(st.width(), st.height(), false);
	bool touched;
	bool touched2 = false;
	do {
		touched = false;
		Status::BorderType destBorder;
		floodFill(*status_, status_->table().destination(), destReachable, &destBorder);
		Status::BorderType border = intersect(status_->border(), destBorder);
		for (Status::BorderType::const_iterator it = border.begin();
				it != border.end(); ++it) {
			if (pushStone(*it)) {
				touched = true;
			}
		}
		if (touched)
			touched2 = true;
	} while (touched);
	return touched2 ? node_ : Node::Ptr();
}

bool InternalPusher::pushStone(const Point &p) {
	if (p == status_->table().destination())
		return true;
//	Point currentTmp(status.currentPos());
	if (pushStoneIter(p, Point::p10))
		return true;
	if (pushStoneIter(p, Point::pm10))
		return true;
	if (pushStoneIter(p, Point::p01))
		return true;
	if (pushStoneIter(p, Point::p0m1))
		return true;
	return false;
}

bool InternalPusher::pushStoneIter(const Point &p, const Point &d) {
	Point pd = p+d;
	Point pmd = p-d;
	if (status_->value(pmd) != ftFloor || status_->value(pd) != ftFloor || !status_->reachable(pmd))
		return false;
	if (status_->currentPos() == pd)
		shiftCurrentPos(*status_);
	int heur1 = calculator_->calculateStone(*status_, p);
	Point currentTmp(status_->currentPos());
	status_->currentPos(p);
	int heur2 = calculator_->calculateStone(*status_, pd);
	if ((heur2 < 0 && (pd != status_->table().destination())) || heur2 >= heur1)
	{
		status_->currentPos(currentTmp);
		return false;
	}
	if (!status_->moveStone(p, pd))
	{
		std::cerr << "Whoopsie doopsie!";
		return false; // should never happen
	}
	node_ = Node::create(status_->state(), p, d, node_, 1, calculator_->calculateStatus(*status_));
	if (pushStone(pd))
		return true;
	node_ = node_->ansector();
	bool couldStepBack = status_->moveStone(pd, p);
	assert(couldStepBack);
	status_->currentPos(currentTmp);
	return false;
}


StonePusher::StonePusher(VisitedStateHolder::Ptr visitedStates, HeurCalculator::Ptr calculator):
		visitedStates_(visitedStates),
		calculator_(calculator)
{
	assert(calculator != NULL);
}

bool StonePusher::expand(const Status &status, boost::shared_ptr<Node> base,
		NodePusher& queue, Dumper::Ptr dumper)
{
	InternalPusher sp(calculator_);
	Node::Ptr node = sp.pushStones(status, base);
	if (node.get() == NULL)
		return false;
	queue.push(node);
	Status st(status.tablePtr());
	std::deque<Node::Ptr> path = pathToBase(node, base);
	for (std::deque<Node::Ptr>::iterator it = path.begin();
			it != path.end(); ++it) {
		st.set(**it);
		if (visitedStates_)
			visitedStates_->push(std::make_pair<const Status&, int>(st, (*it)->costFgv()));
		if (dumper) {
			dumper->push(*it);
		}
	}
	return true;
}


