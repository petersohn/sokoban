#include "BlockListHeurCalculator.h"
#include "Dumper/DumperFunctions.h"
#include "Node.h"
#include "VisitedStateInfo.h"
#include "Status/MockStatus.h"
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
	MockStatus mockStatus(status);
	int result = 0;
	for (const std::unique_ptr<HeurInfo>& subset: *heurList_) {
		if (isSubStatus(subset->first, mockStatus)) {
			result += subset->second;
			for (const Point& p: subset->first.state()) {
				mockStatus.state().removeStone(p);
			}
		}
	}
	for (const Point& p: mockStatus.state()) {
		result += baseCalculator_->calculateStone(status, p);
	}
	if (ancestor) {
		return std::max(result, ancestor->heur() - 1);
	}
	return result;
}


