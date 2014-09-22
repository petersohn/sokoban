#include "ComplexExpander.hpp"
#include "Status/Status.hpp"
#include "Node.hpp"

bool ComplexExpander::expand(const Status &status, std::shared_ptr<Node> base,
		NodePusher& queue, Dumper::Ptr dumper) {
	for (ContainerType::iterator it = funcs_.begin(); it != funcs_.end(); ++it) {
		assert(it->get() != NULL);
		if ((*it)->expand(status, base, queue, dumper))
			return true;
	}
	return false;
}

