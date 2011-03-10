#include "StonePusher.h"
#include "Array.h"
#include "Status.h"
#include "State.h"
#include "VisitedStateInfo.h"
#include <iostream>

class InternalPusher {
public:
	typedef std::deque<std::pair<State, VisitedStateInfo> > PushListType;
private:
	Node::Ptr node_;
	HeurCalculator::Ptr calculator_;
	bool pushStone(Status &status, const Point &p);
	bool pushStoneIter(Status &status, const Point &p, const Point &d);
public:
	InternalPusher(HeurCalculator::Ptr calculator):
		calculator_(calculator) {}
	Node::Ptr pushStones(const Status &st, Node::Ptr base);
};

Node::Ptr InternalPusher::pushStones(const Status &st, Node::Ptr base) {
	node_ = base;
	Status status(st);
	Array<bool> destReachable(st.width(), st.height(), false);
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
				touched = true;
			}
		}
		if (touched)
			touched2 = true;
	} while (touched);
	return touched2 ? node_ : Node::Ptr();
}

bool InternalPusher::pushStone(Status &status, const Point &p) {
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

bool InternalPusher::pushStoneIter(Status &status, const Point &p, const Point &d) {
	Point pd = p+d;
	Point pmd = p-d;
	if (status.value(pmd) != ftFloor || status.value(pd) != ftFloor || !status.reachable(pmd))
		return false;
	if (status.currentPos() == pd)
		shiftCurrentPos(status);
	int heur1 = calculator_->calculateStone(status, p);
	Point currentTmp(status.currentPos());
	status.currentPos(p);
	int heur2 = calculator_->calculateStone(status, pd);
	if ((heur2 < 0 && (pd != status.table().destination())) || heur2 >= heur1)
	{
		status.currentPos(currentTmp);
		return false;
	}
	if (!status.moveStone(p, pd))
	{
		std::cerr << "Whoopsie doopsie!";
		return false; // should never happen
	}
	node_ = Node::create(status.state(), p, d, node_, 1, calculator_->calculateStatus(status));
	if (pushStone(status, pd))
		return true;
	node_ = node_->ansector();
	bool couldStepBack = status.moveStone(pd, p);
	assert(couldStepBack);
	status.currentPos(currentTmp);
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


