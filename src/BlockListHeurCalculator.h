#ifndef BLOCKLISTHEURCALCULATOR_H_
#define BLOCKLISTHEURCALCULATOR_H_

#include "TableHeurCalculator.h"
#include "IndexedStatusList.h"
#include "HeurInfo.h"

class BlocklistHeurCalculator: public HeurCalculator {
	HeurCalculator::Ptr baseCalculator_;
	std::shared_ptr<std::vector<HeurInfo>> heurList_;
	FixedTable::Ptr table_;
public:
	BlocklistHeurCalculator(
			const HeurCalculator::Ptr& baseCalculator,
			std::shared_ptr<std::vector<HeurInfo>> heurList,
			FixedTable::Ptr table):
				heurList_(heurList),
				baseCalculator_(baseCalculator),
				table_(table)
	{
	}

	virtual int calculateStone(const Status &status, const Point &p);
	virtual int calculateStatus(const Status &status);
};


#endif /* BLOCKLISTHEURCALCULATOR_H_ */
