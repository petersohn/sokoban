#include "ExtendedCorridorChecker.hpp"
#include "Array.hpp"
#include "Status/Status.hpp"
#include "Status/floodFill.hpp"
#include "FieldType.hpp"

void ExtendedCorridorCheckerStrategy::floodFill(const Status& status,
		Point p0, Array<bool>& result, MinMax& minmax)
{
	Status::BorderType border;
	::floodFill(status, p0, result, minmax, border);
	simpleStatus.reset(new Status{status.table(),
		State{std::move(std::move(border))}, status.currentPos()});
}

bool ExtendedCorridorCheckerStrategy::checkCorridorEnding(const Status & status,
		Point  p0, Point  side) const
{
	Point p1 = p0 + side;
	Point pm1 = p0 - side;
	return status.value(p0) != FieldType::wall &&
			status.value(p1) == FieldType::floor &&
			status.value(pm1) == FieldType::floor &&
			(isMovableTo(status, p1, pm1) || isMovableTo(status, pm1, p1));
}

