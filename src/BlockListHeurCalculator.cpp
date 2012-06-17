#include "BlockListHeurCalculator.h"
#include "DumperFunctions.h"
#include <boost/range/algorithm.hpp>
#include <boost/range/numeric.hpp>

int BlocklistHeurCalculator::calculateStone(const Status &status, const Point &p)
{
	assert(status.tablePtr() == table_);
	return baseCalculator_->calculateStone(status, p);
}

int BlocklistHeurCalculator::calculateStatus(const Status &status)
{
	assert(status.tablePtr() == table_);
	State state = status.state();
	int result = 0;
	for (const HeurInfo& subset: *heurList_) {
		if (status.reachable(subset.info_.firstReachable()) &&
				isSubState(subset.info_.state(), state)) {
			result += subset.heur_;
			for (const Point& p: subset.info_.state()) {
				state.removeStone(p);
			}
		}
	}
	for (const Point& p: state) {
		result += baseCalculator_->calculateStone(status, p);
	}
	return result;
}

