#ifndef BLOCKLISTHEURCALCULATOR_H_
#define BLOCKLISTHEURCALCULATOR_H_

#include "HeurInfo.hpp"
#include "SubStatusHeurCalculator.hpp"

class BlockListHeurList {
	HeurList heurList_;
	HeurList::const_iterator iterator_;
public:
	template <typename HeurListType>
	BlockListHeurList(const HeurListType& heurList):
		heurList_(std::begin(heurList), std::end(heurList)) {}

	void start()
	{
		iterator_ = heurList_.begin();
	}

	const HeurInfo* operator()(const PseudoStatus&)
	{
		return iterator_ == heurList_.end() ? nullptr : &*(iterator_++);
	}
};

using BlocklistHeurCalculator = SubStatusHeurCalculator<BlockListHeurList>;

#endif /* BLOCKLISTHEURCALCULATOR_H_ */
