#include "ProblemHeurCalculator.h"

int ProblemHeurCalculator::calculate(const Status & status)
{
	if (problem_ != status.problemPtr()) {
		problem_ = status.problemPtr();
		init();
	}
	calculate();
}

