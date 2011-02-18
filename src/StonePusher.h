#ifndef STONEPUSHER_H_
#define STONEPUSHER_H_

#include "Node.h"
#include "HeurCalculator.h"
#include <deque>

class VisitedState;

class StonePusher {
	std::deque<VisitedState> pushList_;
	Node::Ptr node_;
	HeurCalculator::Ptr calculator_;
	bool pushStone(Status &status, int stone);
	bool pushStoneIter(Status &status, int stone, const Point &d);
public:
	StonePusher(HeurCalculator::Ptr calculator):
		calculator_(calculator) {}
	Node::Ptr pushStones(const Status &st, Node::Ptr base);
	const std::deque<VisitedState>& pushList() { return pushList_; }
};

#endif /* STONEPUSHER_H_ */
