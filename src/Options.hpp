#ifndef OPTIONS_H_
#define OPTIONS_H_

#include "Compare.hpp"

#include "util/LazyArgumentEnum.hpp"

#include <exception>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

LAZY_ARGUMENT_ENUM(DumpStyle, dumpStyles,
        (none)(text)(xml)(statistics)(best))
LAZY_ARGUMENT_ENUM(BlockListHeurType, blockListHeurTypes,
    (none)(vector)(decisionTree))
LAZY_ARGUMENT_ENUM(MovableCheckerType, movableCheckerTypes,
        (none)(simple)(extended))
LAZY_ARGUMENT_ENUM(CorridorCheckerType, corridorCheckerTypes,
        (none)(simple)(extended))
LAZY_ARGUMENT_ENUM(DumpFilterType, dumpFilterTypes, (none)(text)(regex))

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

    std::size_t         blocklistDecisionTreeDepth_ = 0;
    std::size_t         blockListDistance_ = 0;
    BlockListHeurType   blocklistHeurCalculatorType_ = BlockListHeurType::none;
    std::size_t         blockListStones_ = 0;
    std::size_t         chokePointDistance_ = 2;
    std::size_t         chokePointDistantNum_ = 2;
    std::size_t         chokePointNum_ = 0;
    CompareList         compare_;
    CorridorCheckerType corridorCheckerType_ = CorridorCheckerType::simple;
    bool                distanceLimiter_ = false;
    std::string         dumpFilename_;
    std::string         dumpFilter_;
    DumpFilterType      dumpFilterType_ = DumpFilterType::none;
    DumpStyle           dumpStyle_ = DumpStyle::none;
    std::size_t         expandedNodeLimit_ = 0;
    float               heurMultiplier_ = 1.0f;
    std::size_t         maxDecisionTreeDepth_ = 10;
    std::size_t         maxHeurListSize_ = 0;
    MovableCheckerType  movableCheckerType_ = MovableCheckerType::simple;
    std::size_t         numThreads_ = 1;
    std::string         outputFormat_ = "";
    bool                parallelExpand_ = false;
    std::string         partitionsDumpFilename_ = "partitions.dump";
    std::size_t         reverseSearchMaxDepth_ = 2;
    std::string         solutionDumpFilename_ = "solution.dump";
    std::size_t         statusPoolSize_ = 0;
    bool                useAdvancedHeurCalculator_ = true;
    bool                useCheckerForDecisionTree_ = false;
    bool                useStonePusher_ = true;
    std::size_t         workQueueLength_ = 100;

    std::string         filename_;
    std::string         saveProgress_;
    float               saveInterval_ = 0.0;
    std::string         preprocessLoadFilename_;
    std::string         preprocessSaveFilename_;
    bool                preprocessFillBeforeCalculate_ = false;
    std::size_t         test_ = 0;
};

Options parseOptions(int argc, const char* argv[], const char *configFileName);

template <typename Archive>
void serialize(Archive& ar, Options& options, const unsigned int /*version*/)
{
    ar & options.blocklistDecisionTreeDepth_;
    ar & options.blockListDistance_;
    ar & options.blocklistHeurCalculatorType_;
    ar & options.blockListStones_;
    ar & options.chokePointDistance_;
    ar & options.chokePointDistantNum_;
    ar & options.chokePointNum_;
    ar & options.compare_;
    ar & options.corridorCheckerType_;
    ar & options.distanceLimiter_;
    ar & options.dumpFilename_;
    ar & options.dumpFilter_;
    ar & options.dumpFilterType_;
    ar & options.dumpStyle_;
    ar & options.expandedNodeLimit_;
    ar & options.heurMultiplier_;
    ar & options.maxDecisionTreeDepth_;
    ar & options.maxHeurListSize_;
    ar & options.movableCheckerType_;
    //ar & options.numThreads_;
    //ar & options.outputFormat_;
    ar & options.parallelExpand_;
    ar & options.partitionsDumpFilename_;
    ar & options.reverseSearchMaxDepth_;
    //ar & options.solutionDumpFilename_;
    ar & options.statusPoolSize_;
    ar & options.useAdvancedHeurCalculator_;
    ar & options.useCheckerForDecisionTree_;
    ar & options.useStonePusher_;
    //ar & options.workQueueLength_;
}

#endif /* OPTIONS_H_ */
