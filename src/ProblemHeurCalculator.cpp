#include "ProblemHeurCalculator.h"

int ProblemHeurCalculator::calculate(const Status & status) const
{
	if (table_ != status.problem().tablePtr()) {
		table_ = status.problem().tablePtr();
		init();
	}
	doCalculate();
}

