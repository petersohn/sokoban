#include "ComplexChecker.h"
#include "Status.h"

bool ComplexChecker::check(const Status &status, const Point &p0) {
	bool result = true;
	for (ContainerType::iterator it = funcs_.begin(); it != funcs_.end(); ++it) {
		assert(it->get() != NULL);
		if (!(*it)->check(status, p0))
			return false;
	}
	return true;
}
