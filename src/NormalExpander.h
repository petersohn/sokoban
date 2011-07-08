#ifndef NORMALEXPANDER_H_
#define NORMALEXPANDER_H_

#include "Expander.h"
#include "Checker.h"
#include "HeurCalculator.h"
#include "Dumper.h"
#include "NodeFactory.h"

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
	virtual bool expand(const Status &status, boost::shared_ptr<Node> base, NodePusher& queue, Dumper::Ptr dumper);
	int expandedNodes() const { return expandedNodes_; }
};

#endif /* NORMALEXPANDER_H_ */
