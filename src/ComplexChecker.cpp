#include "ComplexChecker.h"
#include "Status.h"

bool ComplexChecker::check(const Status &status, const Point &p0) {
	lastError_ = "";
	for (ContainerType::iterator it = funcs_.begin(); it != funcs_.end(); ++it) {
		assert(it->get() != NULL);
		if (!(*it)->check(status, p0)) {
			lastError_ = (*it)->errorMessage();
			return false;
		}
	}
	return true;
}

const char* ComplexChecker::errorMessage() {
	return lastError_;
}
