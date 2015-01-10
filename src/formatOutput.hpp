#ifndef FORMATOUTPUT_HPP
#define FORMATOUTPUT_HPP

#include "util/LazyArgumentEnum.hpp"
#include "util/StringedEnum.hpp"
#include "util/ExplicitType.hpp"
#include <string>
#include <deque>
#include <memory>
#include <boost/date_time/posix_time/posix_time_duration.hpp>

class Node;
class Table;

LAZY_ARGUMENT_ENUM(SolutionType, solutionTypes,
		(coordinate)(direction)(minimal)(dump))

STRINGED_ENUM(SolutionQuality, (none)(good)(bad))

using ExpandedNodes = util::ExplicitType<struct tag_ExpandedNodes, std::size_t>;
using ProcessorTime = util::ExplicitType<struct tag_ProcessorTime,
	  boost::posix_time::time_duration>;
using RealTime = util::ExplicitType<struct tag_RealTime,
	  boost::posix_time::time_duration>;

struct SolutionData {
		const Table& table;
		const std::deque<std::shared_ptr<Node>>& solution;
		SolutionQuality solutionQuality;
		ExpandedNodes expandedNodes;
		ProcessorTime processorTime;
		RealTime realTime;
};

std::string formatOutput(const std::string& format, const SolutionData& data);


#endif /* FORMATOUTPUT_HPP */
