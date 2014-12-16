#ifndef NORMALEXPANDER_H_
#define NORMALEXPANDER_H_

#include "Expander.hpp"
#include "ComplexChecker.hpp"
#include "Mutexes.hpp"

class VisitedStates;
class HeurCalculator;
class NodeFactory;

class NormalExpander: public Expander {
	friend class InternalExpander;

	std::shared_ptr<VisitedStates> visitedStates_;
	std::shared_ptr<const HeurCalculator> calculator_;
	ComplexChecker checker_;
	std::shared_ptr<NodeFactory> nodeFactory_;
	int maxDepth_;
	bool enableLog_;
	int expandedNodes_;
public:
	NormalExpander(std::shared_ptr<VisitedStates> vs,
			std::shared_ptr<const HeurCalculator> calculator,
			ComplexChecker ch, std::shared_ptr<NodeFactory> nodeFactory,
			bool enableLog = false);
	~NormalExpander();
	void expand(const Status& status, std::shared_ptr<Node> base,
			PrioNodeQueue& queue, std::shared_ptr<Dumper> dumper) override;
	int expandedNodes() const { return expandedNodes_; }
};

#endif /* NORMALEXPANDER_H_ */
