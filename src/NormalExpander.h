#ifndef NORMALEXPANDER_H_
#define NORMALEXPANDER_H_

#include "Expander.h"
#include "Checker.h"
#include "HeurCalculator.h"
#include "Dumper.h"

class NormalExpander: public Expander {
	VisitedStateHolder::Ptr visitedStates_;
	HeurCalculator::Ptr calculator_;
	Checker::Ptr checker_;
	Dumper::Ptr dumper_;
	int maxDepth_;
	bool enableLog_;
	int expandedNodes_;
	void expandNode(const Status &st, const Point &p, const Point &d,
			boost::shared_ptr<Node> base, NodePusher& queue);
public:
	NormalExpander(VisitedStateHolder::Ptr vs, HeurCalculator::Ptr calculator,
			Checker::Ptr ch, Dumper::Ptr dumper, bool enableLog = false);
	~NormalExpander();
	virtual bool expand(const Status &status, boost::shared_ptr<Node> base, NodePusher& queue);
	int expandedNodes() const { return expandedNodes_; }
};

#endif /* NORMALEXPANDER_H_ */
