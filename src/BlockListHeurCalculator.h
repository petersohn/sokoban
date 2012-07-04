#ifndef BLOCKLISTHEURCALCULATOR_H_
#define BLOCKLISTHEURCALCULATOR_H_

#include "TableHeurCalculator.h"
#include "IndexedStatusList.h"
#include "HeurInfo.h"

class BlocklistHeurCalculator: public HeurCalculator {
	HeurCalculator::Ptr baseCalculator_;
	HeurListPtr heurList_;
	FixedTable::Ptr table_;
	bool useIsSubStatus_;
public:
	BlocklistHeurCalculator(
			const HeurCalculator::Ptr& baseCalculator,
			const HeurListPtr& heurList,
			FixedTable::Ptr table,
			bool useIsSubStatus):
				heurList_(heurList),
				baseCalculator_(baseCalculator),
				table_(table),
				useIsSubStatus_(useIsSubStatus)
	{
	}

	virtual int calculateStone(const Status &status, const Point &p);
	virtual int calculateStatus(const Status &status,
			const std::shared_ptr<Node>& ancestor);
};


#endif /* BLOCKLISTHEURCALCULATOR_H_ */
