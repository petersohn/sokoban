#include "formatOutput.hpp"
#include "Node.hpp"
#include "util/StringFormatter.hpp"
#include "Status/Status.hpp"
#include "Status/Table.hpp"
#include "Dumper/DumperFunctions.hpp"
#include <sstream>
#include <functional>
#include <boost/lexical_cast.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace {

std::string formatSolution(
		const Table& table,
		const std::deque<std::shared_ptr<Node>>& solution,
		const std::vector<std::string>& args)
{
	SolutionType solutionType = SolutionType::direction;

	if (args.size() >= 1) {
		solutionType = solutionTypes().at(args[0]);
	}

	std::ostringstream result;
	for (const auto& node: solution)
	{
		Point from = node->from();
		Point p(from + node->d());
		switch (solutionType) {
		case SolutionType::minimal:
			result << from.x << ',' << from.y << "->" <<
				p.x << ',' << p.y << ' ';
			break;
		case SolutionType::dump:
			dumpNode(result, table, *node);
			break;
		case SolutionType::coordinate:
		case SolutionType::direction:
			result << from << " --> ";
			if (solutionType == SolutionType::coordinate) {
				result << p;
			} else {
				result << direction(node->d());
			}
			result << '\n';
			break;
		}
	}

	return result.str();
}

std::string formatSolutionQuality(SolutionQuality solutionQuality,
		const std::vector<std::string>& args)
{
	std::size_t numericQuality = static_cast<int>(solutionQuality);

	if (numericQuality < args.size()) {
		return args[numericQuality];
	} else {
		return boost::lexical_cast<std::string>(solutionQuality);
	}
}

}

std::string formatOutput(const std::string& format, const SolutionData& data)
{
	using std::placeholders::_1;
	util::StringFormatter::Map actions{
			{"solution", std::bind(formatSolution, std::cref(data.table),
					std::cref(data.solution), _1)},
			{"length", util::genericFormat(data.solution.size())},
			{"quality", std::bind(formatSolutionQuality,
					data.solutionQuality, _1)},
#ifndef NO_UNSAFE_DIAGNOSTICS
			{"status-moved", util::genericFormat(Status::moveCount)},
			{"status-copied", util::genericFormat(Status::copyCount)},
			{"calculate-reachable-called",
					util::genericFormat(Status::calculateReachableCount)},
#else
			{"status-moved", util::genericFormat(0)},
			{"status-copied", util::genericFormat(0)},
			{"calculate-reachable-called", util::genericFormat(0)},
#endif
			{"expanded-nodes", util::genericFormat(data.expandedNodes)},
			{"total-processor-time",
					util::genericFormat(data.totalTime.value().processorTime)},
			{"total-real-time",
					util::genericFormat(data.totalTime.value().realTime)},
		};
	util::StringFormatter formatter{actions};

	return formatter.formatString(format);
}
