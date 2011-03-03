#ifndef NORMALEXPANDER_H_
#define NORMALEXPANDER_H_

#include "Expander.h"
#include "Checker.h"

class NormalExpander: public Expander {
	VisitedStateHolder::Ptr visitedStates_;
	Checker::Ptr checker_;
	int maxDepth_;
	void expandNode(const Status &status, boost::shared_ptr<Node> base, NodePusher& queue);
public:
	NormalExpander(VisitedStateHolder::Ptr vs, Checker::Ptr ch);
	bool expand(const Status &status, boost::shared_ptr<Node> base, NodePusher& queue);
};

#endif /* NORMALEXPANDER_H_ */
