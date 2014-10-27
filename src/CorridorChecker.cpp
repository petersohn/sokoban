#include "CorridorChecker.hpp"
#include "Array.hpp"
#include "Status/Status.hpp"
#include "Status/StatusUtils.hpp"

bool CorridorChecker::check(const Status& status, Point  p0) const
{
	Array<bool> unchecked(3, 3, true);
	Point p1;
	for (p1.x = 0; p1.x < 3; p1.x++)
		for (p1.y = 0; p1.y < 3; p1.y++) {
			Point p = p0 + p1 + Point::pm1m1;
			if (!unchecked[p1] || status.value(p) != ftFloor || status.reachable(p))
				continue;
			Array<bool> reach(status.width(), status.height(), false);
			MinMax minmax;
			floodFill(status, p, reach, nullptr, &minmax);
			if (!reach[status.table().destination()]) {
				if (minmax.minX == minmax.maxX && minmax.minY == minmax.maxY) {
					if (!checkCorridorEnding(status,
								Point(minmax.minX, minmax.minY - 1), Point::p10) &&
						!checkCorridorEnding(status,
								Point(minmax.minX, minmax.maxY + 1), Point::p10) &&
						!checkCorridorEnding(status,
								Point(minmax.minX - 1, minmax.minY), Point::p01) &&
						!checkCorridorEnding(status,
								Point(minmax.maxX + 1, minmax.minY), Point::p01)) {
						return false;
					}
				} else
				if (minmax.minX == minmax.maxX) {
					if (!checkCorridorEnding(status,
								Point(minmax.minX, minmax.minY - 1), Point::p10) &&
						!checkCorridorEnding(status,
								Point(minmax.minX, minmax.maxY + 1), Point::p10)) {
						return false;
					}
				} else
				if (minmax.minY == minmax.maxY) {
					if (!checkCorridorEnding(status,
								Point(minmax.minX - 1, minmax.minY), Point::p01) &&
						!checkCorridorEnding(status,
								Point(minmax.maxX + 1, minmax.minY), Point::p01)) {
						return false;
					}
				}
			}
			Point p2;
			for (p2.x = 0; p2.x < 3; p2.x++)
				for (p2.y = 0; p2.y < 3; p2.y++) {
					Point pp = p0 + p2 + Point::pm1m1;
					if (arrayAt<bool>(reach, pp, false))
						unchecked[p2] = false;
				}
		}
	return true;
}

bool CorridorChecker::checkCorridorEnding(const Status & status,
			Point  p0, Point  side) const
{
	Point p1 = p0 + side;
	Point pm1 = p0 - side;
	return status.value(p0) != ftWall &&
			status.value(p1) == ftFloor &&
			status.value(pm1) == ftFloor &&
			(calculator_->calculateStone(status, p1) >= 0 ||
			 calculator_->calculateStone(status, pm1) >= 0);
}

const char* CorridorChecker::errorMessage() const
{
	return "corridor found";
}

