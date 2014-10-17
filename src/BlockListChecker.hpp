#ifndef BLOCKLISTCHECKER_HPP_
#define BLOCKLISTCHECKER_HPP_

#include "Checker.hpp"
#include "IndexedStatusList.hpp"

class BlockListChecker: public Checker {
private:
	IndexedStatusList<int> blockList_;
public:
	BlockListChecker(IndexedStatusList<int> blockList):
			blockList_(std::move(blockList))
	{
	}

	bool check(const Status& status, Point  p) const override;
	const char* errorMessage() const override;
};


#endif /* BLOCKLISTCHECKER_HPP_ */
