#include "StonePusher.h"
#include "Array.h"
#include "Status.h"
#include "State.h"
#include "VisitedStateInfo.h"
#include <iostream>

Node::Ptr StonePusher::pushStones(const Status &st, Node::Ptr base) {
	pushList_.clear();
	node_ = base;
	Status status(st);
	Array<bool> destReachable(st.width(), st.height(), false);
	bool touched;
	bool touched2 = false;
	do {
		touched = false;
		std::deque<int> destBorder;
		floodFill(status, status.table().destination(), destReachable, &destBorder);
		std::deque<int> border = intersect(status.border(), destBorder);
		for (std::deque<int>::const_iterator it = border.begin();
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

bool StonePusher::pushStone(Status &status, int stone) {
	if (status.state()[stone] == status.table().destination())
		return true;
//	Point currentTmp(status.currentPos());
	if (pushStoneIter(status, stone, Point::p10))
		return true;
	if (pushStoneIter(status, stone, Point::pm10))
		return true;
	if (pushStoneIter(status, stone, Point::p01))
		return true;
	if (pushStoneIter(status, stone, Point::p0m1))
		return true;
	return false;
}

bool StonePusher::pushStoneIter(Status &status, int stone, const Point &d) {
	Point p = status.state()[stone];
	Point pd = p+d;
	if (status.value(p-d) != ftFloor || status.value(pd) != ftFloor || !status.reachable(p-d))
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
	if (!status.moveStone(stone, pd))
	{
		std::cerr << "Whoopsie doopsie!";
		return false; // should never happen
	}
	node_ = Node::create(status.state(), stone, d, node_, 1, calculator_->calculateStatus(status));
	pushList_.push_back(std::make_pair(status.state(),
			VisitedStateInfo(status.currentPos(), node_->costFgv())));
	if (pushStone(status, stone))
		return true;
	node_ = node_->ansector();
	pushList_.pop_back();
	bool couldStepBack = status.moveStone(stone, p);
	assert(couldStepBack);
	status.currentPos(currentTmp);
	return false;
}

