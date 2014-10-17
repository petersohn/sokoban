#include "ComplexChecker.hpp"
#include "Status/Status.hpp"

bool ComplexChecker::check(const Status &status, Point p0) const {
#ifndef NO_UNSAFE_DIAGNOSTICS
	lastError_ = "";
#endif
	for (const auto& func: funcs_) {
		assert(func.get() != NULL);
		if (!func->check(status, p0)) {
#ifndef NO_UNSAFE_DIAGNOSTICS
			lastError_ = func->errorMessage();
#endif
			return false;
		}
	}
	return true;
}

const char* ComplexChecker::errorMessage() const {
#ifndef NO_UNSAFE_DIAGNOSTICS
	return lastError_;
#else
	return "check failed";
#endif
}
