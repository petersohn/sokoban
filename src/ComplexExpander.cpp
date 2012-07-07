#include "ComplexExpander.h"
#include "Status/Status.h"
#include "Node.h"

bool ComplexExpander::expand(const Status &status, std::shared_ptr<Node> base,
		NodePusher& queue, Dumper::Ptr dumper) {
	for (ContainerType::iterator it = funcs_.begin(); it != funcs_.end(); ++it) {
		assert(it->get() != NULL);
		if ((*it)->expand(status, base, queue, dumper))
			return true;
	}
	return false;
}

