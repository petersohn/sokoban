#ifndef FORMATOUTPUT_HPP
#define FORMATOUTPUT_HPP

#include "util/LazyArgumentEnum.hpp"
#include "util/StringedEnum.hpp"
#include "util/ExplicitType.hpp"
#include "util/TimeMeter.hpp"
#include <string>
#include <deque>
#include <memory>
#include <boost/date_time/posix_time/posix_time_duration.hpp>

class Node;
class Table;

LAZY_ARGUMENT_ENUM(SolutionType, solutionTypes,
        (coordinate)(direction)(minimal)(dump))
LAZY_ARGUMENT_ENUM(TimeFormatType, timeFormatTypes,
        (full)(seconds)(mseconds))

STRINGED_ENUM(SolutionQuality, (none)(good)(bad))

using ExpandedNodes = util::ExplicitType<struct tag_ExpandedNodes, std::size_t>;
using TotalTime = util::ExplicitType<struct tag_ProcessorTime, util::TimerData>;
using ChokePointFindingTime =
        util::ExplicitType<struct tag_ChokePointFindingTime, util::TimerData>;
using PreprocessingIterationTime =
        util::ExplicitType<struct tag_PreprocessingIterationTime,
        util::TimerData>;

struct SolutionData {
    const Table& table;
    const std::deque<std::shared_ptr<Node>>& solution;
    SolutionQuality solutionQuality;
    ExpandedNodes expandedNodes;
    TotalTime totalTime;
    ChokePointFindingTime chokePointFinderTime;
    PreprocessingIterationTime preprocessingIterationTime;
};

std::string formatOutput(const std::string& format, const SolutionData& data);


#endif /* FORMATOUTPUT_HPP */
