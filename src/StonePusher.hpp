#ifndef STONEPUSHER_H_
#define STONEPUSHER_H_

#include "Node.hpp"
#include "HeurCalculator.hpp"
#include "Expander.hpp"
#include "NodeFactory.hpp"
#include <deque>
#include "PrioNodeQueueFwd.hpp"

class State;
class VisitedStates;

class StonePusher: public Expander {
	Expander::Ptr expander_;
	std::shared_ptr<VisitedStates> visitedStates_;
	HeurCalculator::Ptr calculator_;
	NodeFactory::Ptr nodeFactory_;
public:
	StonePusher(Expander::Ptr expander,
			std::shared_ptr<VisitedStates> visitedStates,
			HeurCalculator::Ptr calculator, NodeFactory::Ptr nodeFactory);
	void expand(const Status& status, std::shared_ptr<Node> base,
			PrioNodeQueue& queue, Dumper::Ptr dumper) override;
};



#endif /* STONEPUSHER_H_ */
