#include "BlockListHeurCalculator.h"
#include "Dumper/DumperFunctions.h"
#include "Node.h"
#include "VisitedStateInfo.h"
#include "Status/PseudoStatus.h"
#include "Status/StatusUtils.h"
#include <boost/range/algorithm.hpp>
#include <boost/range/numeric.hpp>

int BlocklistHeurCalculator::calculateStone(const Status &status, const Point &p)
{
	assert(status.tablePtr() == table_);
	return baseCalculator_->calculateStone(status, p);
}

int BlocklistHeurCalculator::calculateStatus(
		const Status &status,
		const MoveDescriptor* /*moveDescriptor*/,
		const std::shared_ptr<Node>& ancestor)
{
	assert(status.tablePtr() == table_);
	PseudoStatus mockStatus(status);
	int result = 0;
	{
		boost::unique_lock<MutexType> lock(mutex_);
		for (const HeurInfoConstPtr& subset: heurList_) {
			if (isSubStatus(subset->first, mockStatus)) {
				result += subset->second;
				for (const Point& p: subset->first.state()) {
					mockStatus.state().removeStone(p);
				}
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


