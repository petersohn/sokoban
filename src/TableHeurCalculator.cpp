#include "TableHeurCalculator.h"

int TableHeurCalculator::calculateStatus(const Status & status,
			const std::shared_ptr<Node>& /*ancestor*/)
{
	checkTable(status);
	int result = 0;
	for (State::const_iterator it = status.state().begin();
			it != status.state().end(); ++it) {
		int val = doCalculateStone(status, *it);
		if (val < 0)
			return -1;
		result += val;
	}
	return result;
}

int TableHeurCalculator::calculateStone(const Status &status, const Point &p)
{
	checkTable(status);
	return doCalculateStone(status, p);
}



