#ifndef NORMALEXPANDER_H_
#define NORMALEXPANDER_H_

#include "Expander.hpp"
#include "Checker.hpp"
#include "HeurCalculator.hpp"
#include "Dumper/Dumper.hpp"
#include "NodeFactory.hpp"
#include "Mutexes.hpp"

class NormalExpander: public Expander {
	friend class InternalExpander;

	VisitedStateHolder::Ptr visitedStates_;
	HeurCalculator::Ptr calculator_;
	Checker::Ptr checker_;
	NodeFactory::Ptr nodeFactory_;
	int maxDepth_;
	bool enableLog_;
	int expandedNodes_;
public:
	NormalExpander(VisitedStateHolder::Ptr vs, HeurCalculator::Ptr calculator,
			Checker::Ptr ch, NodeFactory::Ptr nodeFactory, bool enableLog = false);
	~NormalExpander();
	virtual bool expand(const Status &status, std::shared_ptr<Node> base, NodePusher& queue, Dumper::Ptr dumper);
	int expandedNodes() const { return expandedNodes_; }
};

#endif /* NORMALEXPANDER_H_ */
