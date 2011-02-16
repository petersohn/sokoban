#include "TableHeurCalculator.h"
#include "Status.h"

int TableHeurCalculator::calculateStatus(const Status & status)
{
	if (table_ != status.tablePtr()) {
		table_ = status.tablePtr();
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
	if (table_ != status.tablePtr()) {
		table_ = status.tablePtr();
		init();
	}
	return doCalculateStone(status, p);
}



