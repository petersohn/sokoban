#ifndef BLOCKLISTCHECKER_HPP_
#define BLOCKLISTCHECKER_HPP_

#include "Checker.hpp"
#include "IndexedStatusList.hpp"

class BlockListChecker: public Checker {
private:
	IndexedStatusList blockList_;
public:
	BlockListChecker(IndexedStatusList blockList):
			blockList_(std::move(blockList))
	{
	}

	bool check(const Status& status, Point  p) const override;
	const char* errorMessage() const override;
};


#endif /* BLOCKLISTCHECKER_HPP_ */
