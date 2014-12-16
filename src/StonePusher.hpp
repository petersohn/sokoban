#ifndef STONEPUSHER_H_
#define STONEPUSHER_H_

#include "HeurCalculator.hpp"
#include "Expander.hpp"
#include "NodeFactory.hpp"
#include <deque>
#include "PrioNodeQueueFwd.hpp"

class State;
class Node;
class VisitedStates;

class StonePusher: public Expander {
	Expander::Ptr expander_;
	std::shared_ptr<VisitedStates> visitedStates_;
	std::shared_ptr<const HeurCalculator> calculator_;
	NodeFactory::Ptr nodeFactory_;
public:
	StonePusher(Expander::Ptr expander,
			std::shared_ptr<VisitedStates> visitedStates,
			std::shared_ptr<const HeurCalculator> calculator, NodeFactory::Ptr nodeFactory);
	void expand(const Status& status, std::shared_ptr<Node> base,
			PrioNodeQueue& queue, Dumper::Ptr dumper) override;
};



#endif /* STONEPUSHER_H_ */
