#include "BasicHeurCalculator.h"


void BasicHeurCalculator::init() {
	distances_.reset(table().width(), table().height(), 0);
	distances_[table().destination()] = 1;
	bool touched;
	Point p;
	do {
		touched = false;
		for (Point  p: arrayRange(table())) {
			if (distances_[p] == 0)
				continue;
			int newDist = distances_[p]+1;
			if (checkDistance(p, Point::p10, newDist))
				touched = true;
			if (checkDistance(p, Point::pm10, newDist))
				touched = true;
			if (checkDistance(p, Point::p01, newDist))
				touched = true;
			if (checkDistance(p, Point::p0m1, newDist))
				touched = true;
		}
	} while (touched);
	for (Point  p: arrayRange(table())) {
		distances_[p]--;
	}
}

int BasicHeurCalculator::doCalculateStone(const Status &status, Point p)
{
	return distances_[p];
}

bool BasicHeurCalculator::checkDistance(Point  p, Point  d, int dist)
{
	Point pd = p+d;
	bool result =
			! table().wall(pd) &&
			!table().wall(p+d*2) &&
			(distances_[pd] == 0 || distances_[pd] > dist);
	if (result)
		distances_[pd] = dist;
	return result;
}
