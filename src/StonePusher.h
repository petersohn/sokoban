#ifndef STONEPUSHER_H_
#define STONEPUSHER_H_

#include "Node.h"
#include <deque>

class VisitedState;

class StonePusher {
	std::deque<VisitedState> pushList_;
	Node::Ptr node_;
public:
	bool pushStones(const Status &st, Node::Ptr base);
};

#endif /* STONEPUSHER_H_ */
