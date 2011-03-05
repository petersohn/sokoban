#include "ComplexExpander.h"

bool ComplexExpander::expand(const Status &status, boost::shared_ptr<Node> base, NodePusher& queue) {
	ExpandParams p(status, base, queue);
	funcs_(p);
}

