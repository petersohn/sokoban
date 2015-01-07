#include "formatOutput.hpp"
#include "Node.hpp"
#include "util/StringFormatter.hpp"
#include <sstream>
#include <functional>

namespace {

std::string formatSolution(const std::deque<std::shared_ptr<Node>>& solution,
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
		result << from << " --> ";
		switch (solutionType) {
		case SolutionType::coordinate:
			result << p;
			break;
		case SolutionType::direction:
			result << direction(node->d());
			break;
		}
		result << '\n';
	}

	return result.str();
}

}

std::string formatOutput(const std::string& format,
		const std::deque<std::shared_ptr<Node>>& solution)
{
	using std::placeholders::_1;
	util::StringFormatter::Map actions{
            {"solution", std::bind(formatSolution, std::cref(solution), _1)}
        };
	util::StringFormatter formatter{actions};

	return formatter.formatString(format);
}
