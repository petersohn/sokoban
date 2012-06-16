#include "BlockListHeurCalculator.h"
#include "DumperFunctions.h"
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
		int added = *boost::min_element(addedValues);
//		if (added > 0) {
//			std::cerr << "Added: " << baseResult << " + " <<
//					added << " = " << baseResult + added << std::endl;
//			dumpStatus(std::cerr, status, "", &status.reachableArray());
//		}
		return baseResult + added;
	}
	return baseResult;
}

