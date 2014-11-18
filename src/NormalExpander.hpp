#ifndef NORMALEXPANDER_H_
#define NORMALEXPANDER_H_

#include "Expander.hpp"
#include "ComplexChecker.hpp"
#include "HeurCalculator.hpp"
#include "Dumper/Dumper.hpp"
#include "NodeFactory.hpp"
#include "Mutexes.hpp"

class VisitedStates;

class NormalExpander: public Expander {
	friend class InternalExpander;

	std::shared_ptr<VisitedStates> visitedStates_;
	HeurCalculator::Ptr calculator_;
	ComplexChecker checker_;
	NodeFactory::Ptr nodeFactory_;
	int maxDepth_;
	bool enableLog_;
	int expandedNodes_;
public:
	NormalExpander(std::shared_ptr<VisitedStates> vs,
			HeurCalculator::Ptr calculator,
			ComplexChecker ch, NodeFactory::Ptr nodeFactory,
			bool enableLog = false);
	~NormalExpander();
	void expand(const Status& status, std::shared_ptr<Node> base,
			PrioNodeQueue& queue, Dumper::Ptr dumper) override;
	int expandedNodes() const { return expandedNodes_; }
};

#endif /* NORMALEXPANDER_H_ */
