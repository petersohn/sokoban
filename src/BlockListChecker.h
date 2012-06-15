#ifndef BLOCKLISTCHECKER_HPP_
#define BLOCKLISTCHECKER_HPP_

#include "Checker.h"
#include "IndexedStatusList.h"

class BlockListChecker: public Checker {
private:
	std::shared_ptr<IndexedStatusList<int>> blockList_;
	FixedTable::Ptr table_;
public:
	BlockListChecker(
			std::shared_ptr<IndexedStatusList<int>> blockList,
			FixedTable::Ptr table):
				blockList_(blockList),
				table_(table)
	{
	}

	virtual bool check(const Status& status, const Point& p);
	virtual const char* errorMessage();
};


#endif /* BLOCKLISTCHECKER_HPP_ */
