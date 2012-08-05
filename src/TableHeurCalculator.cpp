#include "TableHeurCalculator.h"
#include "Node.h"
#include <iostream>

int TableHeurCalculator::calculateStatus(
			const Status &status,
			const MoveDescriptor* moveDescriptor,
			const std::shared_ptr<Node>& ancestor)
{
	checkTable(status);
	if (moveDescriptor && ancestor) {
		Status oldStatus(status.tablePtr(), *ancestor);
		int oldHeur = doCalculateStone(oldStatus, moveDescriptor->from_);
		int newHeur = doCalculateStone(status, moveDescriptor->from_ + moveDescriptor->d_);
		if (newHeur < 0) {
			return -1;
		}
		return ancestor->heur() -
				oldHeur +
				newHeur;
	} else
	{
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
}

int TableHeurCalculator::calculateStone(const Status &status, const Point &p)
{
	checkTable(status);
	return doCalculateStone(status, p);
}



