#include "CorridorChecker.hpp"
#include "Array.hpp"
#include "Status/Status.hpp"
#include "Status/floodFill.hpp"
#include "FieldType.hpp"

bool CorridorCheckerStrategy::checkCorridorEnding(const Status & status,
			Point  p0, Point  side) const
{
	Point p1 = p0 + side;
	Point pm1 = p0 - side;
	return status.value(p0) != FieldType::wall &&
			status.value(p1) == FieldType::floor &&
			status.value(pm1) == FieldType::floor &&
			(calculator_->calculateStone(status, p1) >= 0 ||
			 calculator_->calculateStone(status, pm1) >= 0);
}

void CorridorCheckerStrategy::floodFill(const Status& status,
		Point p0, Array<bool>& result, MinMax& minmax)
{
	::floodFill(status, p0, result, minmax);
}

