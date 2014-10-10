#ifndef BLOCKLISTCHECKER_HPP_
#define BLOCKLISTCHECKER_HPP_

#include "Checker.hpp"
#include "IndexedStatusList.hpp"

class BlockListChecker: public Checker {
private:
	IndexedStatusList<int> blockList_;
	FixedTable::Ptr table_;
public:
	BlockListChecker(
			IndexedStatusList<int> blockList,
			FixedTable::Ptr table):
				blockList_(std::move(blockList)),
				table_(table)
	{
	}

	virtual bool check(const Status& status, Point  p);
	virtual const char* errorMessage();
};


#endif /* BLOCKLISTCHECKER_HPP_ */
