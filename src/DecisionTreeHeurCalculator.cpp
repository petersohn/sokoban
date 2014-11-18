#include "DecisionTreeHeurCalculator.hpp"
#include "Dumper/DumperFunctions.hpp"
#include "Node.hpp"
#include "VisitedStateInfo.hpp"
#include "Status/StatusUtils.hpp"
#include <boost/range/algorithm.hpp>
#include <boost/range/numeric.hpp>
#include <functional>
#include <iostream>

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

