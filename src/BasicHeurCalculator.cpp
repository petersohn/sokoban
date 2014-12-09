#include "BasicHeurCalculator.hpp"


BasicStoneCalculator::BasicStoneCalculator(const Table& table)
{
	distances_.reset(table.width(), table.height(), 0);
	distances_[table.destination()] = 1.0f;
	bool touched;
	Point p;
	do {
		touched = false;
		for (Point  p: arrayRange(table)) {
			if (distances_[p] == 0)
				continue;
			float newDist = distances_[p]+1.0f;
			if (checkDistance(table, p, Point::p10, newDist))
				touched = true;
			if (checkDistance(table, p, Point::pm10, newDist))
				touched = true;
			if (checkDistance(table, p, Point::p01, newDist))
				touched = true;
			if (checkDistance(table, p, Point::p0m1, newDist))
				touched = true;
		}
	} while (touched);
	for (auto& distance: distances_) {
		distance -= 1.0f;
	}
}

float BasicStoneCalculator::operator()(const Status& /*status*/, Point p) const
{
	return distances_[p];
}

bool BasicStoneCalculator::checkDistance(const Table& table, Point  p, Point  d,
		int dist)
{
	Point pd = p+d;
	bool result =
			! table.wall(pd) &&
			!table.wall(p+d*2) &&
			(distances_[pd] == 0 || distances_[pd] > dist);
	if (result)
		distances_[pd] = dist;
	return result;
}
