#include "ComplexExpander.h"
#include "Status.h"
#include "Node.h"

bool ComplexExpander::expand(const Status &status, boost::shared_ptr<Node> base, NodePusher& queue) {
	for (ContainerType::iterator it = funcs_.begin(); it != funcs_.end(); ++it) {
		assert(it->get() != NULL);
		if ((*it)->expand(status, base, queue))
			return true;
	}
	return false;
}

