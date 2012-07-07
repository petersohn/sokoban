#include "BlockListHeurCalculator.h"
#include "DumperFunctions.h"
#include "Node.h"
#include "VisitedStateInfo.h"
#include <boost/range/algorithm.hpp>
#include <boost/range/numeric.hpp>

int BlocklistHeurCalculator::calculateStone(const Status &status, const Point &p)
{
	assert(status.tablePtr() == table_);
	return baseCalculator_->calculateStone(status, p);
}

int BlocklistHeurCalculator::calculateStatus(const Status &status,
			const std::shared_ptr<Node>& ancestor)
{
	assert(status.tablePtr() == table_);
	State state = status.state();
	int result = 0;
	for (const std::unique_ptr<HeurInfo>& subset: *heurList_) {
		if (subset->status_.reachable(status.currentPos()) &&
						isSubState(subset->status_.state(), state)) {
			result += subset->heur_;
			for (const Point& p: subset->status_.state()) {
				state.removeStone(p);
			}
		}
	}
	for (const Point& p: state) {
		result += baseCalculator_->calculateStone(status, p);
	}
	if (ancestor) {
		return std::max(result, ancestor->heur() - 1);
	}
	return result;
}

