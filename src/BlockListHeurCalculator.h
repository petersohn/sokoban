#ifndef BLOCKLISTHEURCALCULATOR_H_
#define BLOCKLISTHEURCALCULATOR_H_

#include "TableHeurCalculator.h"
#include "IndexedStatusList.h"

class BlocklistHeurCalculator: public TableHeurCalculator {
	HeurCalculator::Ptr baseCalculator_;
	std::shared_ptr<IndexedStatusList<int>> heurList_;
	FixedTable::Ptr table_;

	virtual void init();
	virtual int doCalculateStone(const Status &status, const Point &p);
public:
	BlocklistHeurCalculator(
			const HeurCalculator::Ptr& baseCalculator,
			std::shared_ptr<IndexedStatusList<int>> heurList,
			FixedTable::Ptr table):
				heurList_(heurList),
				baseCalculator_(baseCalculator),
				table_(table)
	{
	}
};


#endif /* BLOCKLISTHEURCALCULATOR_H_ */
