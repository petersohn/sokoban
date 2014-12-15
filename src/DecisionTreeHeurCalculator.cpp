#include "DecisionTreeHeurCalculator.hpp"

std::vector<Point>
DecisionTreeHeurListFactory::pointList(const Table& table)
{
	std::vector<Point> result;
	for (Point  p: arrayRange(table)) {
		if (!table.wall(p)) {
			result.push_back(p);
		}
	}
	return result;
}

