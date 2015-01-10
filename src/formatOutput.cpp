#include "formatOutput.hpp"
#include "Node.hpp"
#include "util/StringFormatter.hpp"
#include "Status/Status.hpp"
#include "Status/Table.hpp"
#include "Dumper/DumperFunctions.hpp"
#include <sstream>
#include <functional>
#include <boost/lexical_cast.hpp>

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

std::string formatSolutionLength(const std::deque<std::shared_ptr<Node>>& solution,
		const std::vector<std::string>&)
{
	return boost::lexical_cast<std::string>(solution.size());
}

#ifndef NO_UNSAFE_DIAGNOSTICS
std::string formatStatusMoved(const std::vector<std::string>&)
{
	return boost::lexical_cast<std::string>(Status::moveCount);
}

std::string formatStatusCopied(const std::vector<std::string>&)
{
	return boost::lexical_cast<std::string>(Status::copyCount);
}

std::string formatCalculateReachableCalled(const std::vector<std::string>&)
{
	return boost::lexical_cast<std::string>(Status::calculateReachableCount);
}
#else
std::string formatStatusMoved(const std::vector<std::string>&)
{
	return "0";
}

std::string formatStatusCopied(const std::vector<std::string>&)
{
	return "0";
}

std::string formatCalculateReachableCalled(const std::vector<std::string>&)
{
	return "0";
}
#endif

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

std::string formatExpandedNodes(ExpandedNodes expandedNodes,
		const std::vector<std::string>&)
{
	return boost::lexical_cast<std::string>(expandedNodes.value());
}
}

std::string formatOutput(const std::string& format,
		const Table& table,
		const std::deque<std::shared_ptr<Node>>& solution,
		SolutionQuality solutionQuality,
		ExpandedNodes expandedNodes)
{
	using std::placeholders::_1;
	util::StringFormatter::Map actions{
            {"solution", std::bind(formatSolution, std::cref(table),
					std::cref(solution), _1)},
			{"length", std::bind(formatSolutionLength, std::cref(solution), _1)},
			{"quality", std::bind(formatSolutionQuality, solutionQuality, _1)},
			{"status-moved", formatStatusMoved},
			{"status-copied", formatStatusCopied},
			{"calculate-reachable-called", formatCalculateReachableCalled},
			{"expanded-nodes", std::bind(formatExpandedNodes, expandedNodes, _1)}
        };
	util::StringFormatter formatter{actions};

	return formatter.formatString(format);
}
