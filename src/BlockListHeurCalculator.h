#ifndef BLOCKLISTHEURCALCULATOR_H_
#define BLOCKLISTHEURCALCULATOR_H_

#include "HeurCalculator.h"
#include "IndexedStatusList.h"
#include "HeurInfo.h"
#include "Mutexes.hpp"

class BlocklistHeurCalculator: public HeurCalculator {
	HeurCalculator::Ptr baseCalculator_;
	HeurList heurList_;
	FixedTable::Ptr table_;
	MutexType mutex_;
public:
	template <class HeurListType>
	BlocklistHeurCalculator(
			const HeurCalculator::Ptr& baseCalculator,
			const HeurListType& heurList,
			FixedTable::Ptr table):
				heurList_(std::begin(heurList), std::end(heurList)),
				baseCalculator_(baseCalculator),
				table_(table)
	{
	}

	virtual int calculateStone(const Status &status, const Point &p);
	virtual int calculateStatus(
			const Status &status,
			const MoveDescriptor* /*moveDescriptor*/,
			const std::shared_ptr<Node>& ancestor);
};


#endif /* BLOCKLISTHEURCALCULATOR_H_ */
