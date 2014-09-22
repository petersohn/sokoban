#ifndef OPTIONS_H_
#define OPTIONS_H_

#include <string>
#include <exception>
#include <vector>
#include <unordered_map>
#include <iostream>

class Options {
public:
	enum DumpStyle {dsNone, dsText, dsXML};
	enum CompareMethod {ctTime = 1, ctHeur, ctDepth};
	enum BlockListHeurType {bhNone, bhVector, bhDecisionTree};

	struct Compare {
		CompareMethod type;
		bool reverse;
		Compare(CompareMethod type, bool reverse):
			type(type), reverse(reverse)
		{}
	};
	typedef std::vector<Compare> CompareList;
private:
	DumpStyle dumpStyle_;
	bool oldStyleOutput_;
	bool useStonePusher_;
	bool useMovableChecker_;
	bool useCorridorChecker_;
	bool useAdvancedHeurCalculator_;
	std::size_t statusPoolSize_;
	BlockListHeurType blocklistHeurCalculatorType_;
	bool parallelOuterExpand_;
	std::size_t blockListStones_;
	std::size_t blockListDistance_;
	std::size_t maxHeurListSize_;
	std::size_t numThreads_;
	std::size_t test_;
	std::size_t maxDecisionTreeDepth_;
	bool useCheckerForDecisionTree_;
	CompareList compare_;
	std::string filename_;
public:
	Options(int argc, char **argv, const char *configFileName = NULL);

	DumpStyle dumpStyle() const { return dumpStyle_; }
	bool oldStyleOutput() const { return oldStyleOutput_; }
	bool useStonePusher() const { return useStonePusher_; }
	bool useMovableChecker() const { return useMovableChecker_; }
	bool useCorridorChecker() const { return useCorridorChecker_; }
	bool useAdvancedHeurCalculator() const { return useAdvancedHeurCalculator_; }
	std::size_t statusPoolSize() const { return statusPoolSize_; }
	BlockListHeurType blocklistHeurCalculatorType() const { return blocklistHeurCalculatorType_; }
	bool parallelOuterExpand() const { return parallelOuterExpand_; }
	std::size_t blockListStones() const { return blockListStones_; }
	std::size_t blockListDistance() const { return blockListDistance_; }
	std::size_t maxHeurListSize() const { return maxHeurListSize_; }
	std::size_t numThreads() const { return numThreads_; }
	std::size_t test() const { return test_; }
	std::size_t maxDecisionTreeDepth() const { return maxDecisionTreeDepth_; }
	bool useCheckerForDecisionTree() const { return useCheckerForDecisionTree_; }
	const std::string &filename() const { return filename_; }
	const CompareList& compare() const { return compare_; }
};

#endif /* OPTIONS_H_ */
