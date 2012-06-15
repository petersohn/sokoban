#include "BlockListHeurCalculator.h"
#include <boost/range/algorithm.hpp>

void BlocklistHeurCalculator::init()
{
	assert(tablePtr() == table_);
}

int BlocklistHeurCalculator::doCalculateStone(const Status &status, const Point &p)
{
	std::vector<int> addedValues = heurList_->getSubStatusValues(status, p);
	int baseResult = baseCalculator_->calculateStone(status, p);
	if (!addedValues.empty()) {
		return baseResult +	*boost::min_element(addedValues);
	}
	return baseResult;
}

