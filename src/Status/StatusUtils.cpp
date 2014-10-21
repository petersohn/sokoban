#include "Status/StatusUtils.hpp"
#include "Checker.hpp"
#include "Status/IsStatusPossible.hpp"
#include <stack>

void floodFill(const Status &status, Point p0, Array<bool> &result,
			Status::BorderType *border, MinMax *minmax)
{
	result.fill(false);
	if (minmax != NULL) {
		minmax->minX = status.width();
		minmax->maxX = 0;
		minmax->minY = status.height();
		minmax->maxY = 0;
	}

	std::vector<Point> pointsToVisit;
	pointsToVisit.reserve(status.width()*status.height());
	pointsToVisit.push_back(p0);

	while (!pointsToVisit.empty()) {
		Point p = pointsToVisit.back();
		pointsToVisit.pop_back();

		if (status.value(p) != ftFloor || result[p])
		{
			if (border != NULL && status.value(p) == ftStone)
				border->push_back(p);
		} else {
			result[p] = true;
			if (minmax != NULL) {
				minmax->minX = std::min(minmax->minX, p.x);
				minmax->maxX = std::max(minmax->maxX, p.x);
				minmax->minY = std::min(minmax->minY, p.y);
				minmax->maxY = std::max(minmax->maxY, p.y);
			}
			pointsToVisit.push_back(p+Point::p10);
			pointsToVisit.push_back(p+Point::pm10);
			pointsToVisit.push_back(p+Point::p01);
			pointsToVisit.push_back(p+Point::p0m1);
		}
	}
}

std::vector<Status> getPartitions(const Table& table, const State &state,
			std::size_t maxDepth)
{
	Array<bool> pointsToProcess(table.width(), table.height(), false);
	int numberOfPoints = 0;
	for (Point  p: arrayRange(table)) {
		if (!table.wall(p) && !state[p])
		{
			pointsToProcess[p] = true;
			++numberOfPoints;
		} else
			pointsToProcess[p] = false;
	}
	std::vector<Status> result;
	while (numberOfPoints > 0) {
		Point foundPoint;
		for (Point  p: arrayRange(table)) {
			if (pointsToProcess[p]) {
				foundPoint = p;
				break;
			}
		}
		Status status{table, state};
		status.currentPos(foundPoint);

		for (Point  p: arrayRange(table)) {
			if (status.reachable(p) && pointsToProcess[p])
			{
				pointsToProcess[p] = false;
				--numberOfPoints;
			}
		}

		if (isStatusPossible(status, maxDepth)) {
			result.push_back(std::move(status));
		}
	}
	return result;
}

bool checkStatus(const Checker& checker, const Status& status)
{
	for (Point pp: status.state()) {
		if (!checker.check(status, pp)) {
			return false;
		}
	}
	return true;
}

bool checkState(const Checker& checker, const Table& table, const State& state)
{
	std::vector<Status> partitions = getPartitions(table, state, 0);
	for (const Status& status: partitions) {
		if (checkStatus(checker, status)) {
			return true;
		}
	}
	return false;
}




