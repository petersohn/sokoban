#ifndef STONEPUSHER_H_
#define STONEPUSHER_H_

#include "Node.h"
#include "HeurCalculator.h"
#include "Expander.h"
#include "NodeFactory.h"
#include <deque>

class State;
class VisitedStateInfo;


class StonePusher: public Expander {
	VisitedStateHolder::Ptr visitedStates_;
	HeurCalculator::Ptr calculator_;
	NodeFactory::Ptr nodeFactory_;
public:
	StonePusher(VisitedStateHolder::Ptr visitedStates, HeurCalculator::Ptr calculator,
			NodeFactory::Ptr nodeFactory);
	virtual bool expand(const Status &status, boost::shared_ptr<Node> base,
			NodePusher& queue, Dumper::Ptr dumper);
};



#endif /* STONEPUSHER_H_ */
