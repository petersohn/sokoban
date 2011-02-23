#ifndef STONEPUSHER_H_
#define STONEPUSHER_H_

#include "Node.h"
#include "HeurCalculator.h"
#include <deque>

class State;
class VisitedStateInfo;

class StonePusher {
public:
	typedef std::deque<std::pair<State, VisitedStateInfo> > PushListType;
private:
	Node::Ptr node_;
	HeurCalculator::Ptr calculator_;
	bool pushStone(Status &status, const Point &p);
	bool pushStoneIter(Status &status, const Point &p, const Point &d);
public:
	StonePusher(HeurCalculator::Ptr calculator):
		calculator_(calculator) {}
	Node::Ptr pushStones(const Status &st, Node::Ptr base);
};

#endif /* STONEPUSHER_H_ */
