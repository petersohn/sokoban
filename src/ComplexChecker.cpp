#include "ComplexChecker.h"

bool ComplexChecker::check(const Status &status, const Point &p0) {
	CheckParams p(status, p0);
	funcs_.execute(p, true);
}
