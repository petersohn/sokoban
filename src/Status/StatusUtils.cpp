#include "Status/StatusUtils.hpp"
#include "Checker.hpp"
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

std::vector<Status::Ptr> getPartitions(FixedTable::Ptr table, const State &state)
{
	Array<bool> kell(table->get().width(), table->get().height(), false);
	int kellNum = 0;
	for (Point  p: arrayRange(table->get())) {
		if (!table->get().wall(p) && !state[p])
		{
			kell[p] = true;
			++kellNum;
		} else
			kell[p] = false;
	}
	std::vector<Status::Ptr> result;
	while (kellNum > 0) {
		Point foundPoint;
		for (Point  p: arrayRange(table->get())) {
			if (kell[p]) {
				foundPoint = p;
				break;
			}
		}
		Status::Ptr status(new Status(table, state));
		status->currentPos(foundPoint);
		for (Point  p: arrayRange(table->get())) {
			if (status->reachable(p) && kell[p])
			{
				kell[p] = false;
				--kellNum;
			}
		}
		result.push_back(status);
	}
	return result;
}

bool checkStatus(Checker& checker, const Status& status)
{
	for (Point pp: status.state()) {
		if (!checker.check(status, pp)) {
			return false;
		}
	}
	return true;
}

bool checkState(Checker& checker, const FixedTable::Ptr& table, const State& state)
{
	std::vector<Status::Ptr> partitions = getPartitions(table, state);
	for (const Status::Ptr& status: partitions) {
		if (checkStatus(checker, *status)) {
			return true;
		}
	}
	return false;
}




