#include "TableHeurCalculator.h"
#include "Status.h"

int TableHeurCalculator::calculateStatus(const Status & status) const
{
	if (table_ != status.problem().tablePtr()) {
		table_ = status.problem().tablePtr();
		init();
	}
	int result = 0;
	for (int i = 0; i < status.state().size(); ++i) {
		int val = doCalculateStone(status, status.state()[i]);
		if (val < 0)
			return -1;
		result += val;
	}
	return result;
}

int TableHeurCalculator::calculateStone(const Status &status, const Point &p)
{
	if (table_ != table) {
		table_ = table;
		init();
	}
	return doCalculateStone(status, p);
}



