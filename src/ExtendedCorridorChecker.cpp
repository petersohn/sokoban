#include "ExtendedCorridorChecker.hpp"
#include "Array.hpp"
#include "Status/Status.hpp"
#include "Status/floodFill.hpp"
#include "FieldType.hpp"

bool ExtendedCorridorChecker::check(const Status& status, Point  p0) const
{
	Array<bool> unchecked(3, 3, true);
	Point p1;
	for (p1.x = 0; p1.x < 3; p1.x++)
		for (p1.y = 0; p1.y < 3; p1.y++) {
			Point p = p0 + p1 + Point::pm1m1;
			if (!unchecked[p1] || status.value(p) != FieldType::floor || status.reachable(p))
				continue;
			Array<bool> reach(status.width(), status.height(), false);
			MinMax minmax;
			Status::BorderType border;
			floodFill(status, p, reach, minmax, border);
			if (!reach[status.table().destination()]) {

				Status simpleStatus{status.table(),
					State{std::move(std::move(border))}, status.currentPos()};
				const auto& reachability = simpleStatus.reachableArray();

				if (minmax.minX == minmax.maxX && minmax.minY == minmax.maxY) {
					if (!checkCorridorEnding(status, reachability,
								Point(minmax.minX, minmax.minY - 1), Point::p10) &&
						!checkCorridorEnding(status, reachability,
								Point(minmax.minX, minmax.maxY + 1), Point::p10) &&
						!checkCorridorEnding(status, reachability,
								Point(minmax.minX - 1, minmax.minY), Point::p01) &&
						!checkCorridorEnding(status, reachability,
								Point(minmax.maxX + 1, minmax.minY), Point::p01)) {
						return false;
					}
				} else
				if (minmax.minX == minmax.maxX) {
					if (!checkCorridorEnding(status, reachability,
								Point(minmax.minX, minmax.minY - 1), Point::p10) &&
						!checkCorridorEnding(status, reachability,
								Point(minmax.minX, minmax.maxY + 1), Point::p10)) {
						return false;
					}
				} else
				if (minmax.minY == minmax.maxY) {
					if (!checkCorridorEnding(status, reachability,
								Point(minmax.minX - 1, minmax.minY), Point::p01) &&
						!checkCorridorEnding(status, reachability,
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

namespace {

bool isMovableTo(const Status& status, const Array<bool>& reachability,
		const HeurCalculator& calculator,
		Point from, Point to)
{
	return reachability[from] && calculator.calculateStone(status, to) > 0;
}

}

bool ExtendedCorridorChecker::checkCorridorEnding(const Status & status,
		const Array<bool>& reachability, Point  p0, Point  side) const
{
	Point p1 = p0 + side;
	Point pm1 = p0 - side;
	return status.value(p0) != FieldType::wall &&
			status.value(p1) == FieldType::floor &&
			status.value(pm1) == FieldType::floor &&
			(isMovableTo(status, reachability, *calculator_, p1, pm1) ||
			 isMovableTo(status, reachability, *calculator_, pm1, p1));
}

const char* ExtendedCorridorChecker::errorMessage() const
{
	return "corridor found";
}

