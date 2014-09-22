#ifndef BLOCKLISTCHECKER_HPP_
#define BLOCKLISTCHECKER_HPP_

#include "Checker.hpp"
#include "IndexedStatusList.hpp"

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

	virtual bool check(const Status& status, Point  p);
	virtual const char* errorMessage();
};


#endif /* BLOCKLISTCHECKER_HPP_ */
