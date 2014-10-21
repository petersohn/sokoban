#ifndef OPTIONS_H_
#define OPTIONS_H_

#include <string>
#include <exception>
#include <vector>
#include <unordered_map>
#include <iostream>
#include "util/LazyArgumentEnum.hpp"

LAZY_ARGUMENT_ENUM(DumpStyle, dumpStyles, (none)(text)(xml))
LAZY_ARGUMENT_ENUM(BlockListHeurType, blockListHeurTypes,
	(none)(vector)(decisionTree))

enum class CompareMethod {time = 1, heur, depth};

struct Compare {
	CompareMethod type;
	bool reverse;
	Compare(): type(CompareMethod::time), reverse(false) {}
	Compare(CompareMethod type, bool reverse):
		type(type), reverse(reverse)
	{}
};

inline
bool operator==(const Compare& lhs, const Compare& rhs) {
	return lhs.type == rhs.type && lhs.reverse == rhs.reverse;
}
inline
bool operator!=(const Compare& lhs, const Compare& rhs) {
	return !(lhs == rhs);
}

LAZY_ARGUMENT_PREFIX_MAP(Compare, compares) {
	return {
		{"+time", Compare{CompareMethod::time, false}},
		{"-time", Compare{CompareMethod::time, true}},
		{"+heur", Compare{CompareMethod::heur, false}},
		{"-heur", Compare{CompareMethod::heur, true}},
		{"+depth", Compare{CompareMethod::depth, false}},
		{"-depth", Compare{CompareMethod::depth, true}},
	};
}

std::istream& operator>>(std::istream& is, Compare& value);
std::ostream& operator<<(std::ostream& os, const Compare& value);

struct Options {
	typedef std::vector<Compare> CompareList;

	DumpStyle dumpStyle_ = DumpStyle::none;
	bool oldStyleOutput_ = false;
	bool useStonePusher_ = true;
	bool useMovableChecker_ = true;
	bool useCorridorChecker_ = true;
	bool useAdvancedHeurCalculator_ = true;
	std::size_t statusPoolSize_ = 0;
	BlockListHeurType blocklistHeurCalculatorType_ = BlockListHeurType::none;
	std::size_t blockListStones_ = 0;
	std::size_t blockListDistance_ = 0;
	std::size_t maxHeurListSize_ = 0;
	std::size_t workQueueLength_ = 100;
	std::size_t numThreads_ = 1;
	std::size_t test_ = 0;
	std::size_t maxDecisionTreeDepth_ = 10;
	std::size_t blocklistDecisionTreeDepth_ = 0;
	std::size_t reverseSearchMaxDepth_ = 2;
	bool useCheckerForDecisionTree_ = false;
	CompareList compare_;
	std::string filename_;
};

Options parseOptions(int argc, char **argv, const char *configFileName);

#endif /* OPTIONS_H_ */
