#ifndef STONEPUSHER_H_
#define STONEPUSHER_H_

#include "Expander.hpp"
#include "NodeFactory.hpp"
#include "PrioNodeQueueFwd.hpp"

class State;
class Node;
class VisitedStates;
class HeurCalculator;

class StonePusher: public Expander {
	std::shared_ptr<Expander> expander_;
	std::shared_ptr<VisitedStates> visitedStates_;
	std::shared_ptr<const HeurCalculator> calculator_;
	std::shared_ptr<NodeFactory> nodeFactory_;
public:
	StonePusher(std::shared_ptr<Expander> expander,
			std::shared_ptr<VisitedStates> visitedStates,
			std::shared_ptr<const HeurCalculator> calculator, std::shared_ptr<NodeFactory> nodeFactory);
	void expand(const Status& status, std::shared_ptr<Node> base,
			PrioNodeQueue& queue, std::shared_ptr<Dumper> dumper) override;
};



#endif /* STONEPUSHER_H_ */
