#include "Common.h"
#include "Array.h"
#include <algorithm>

static void floodFillIter(const Array<FieldType> &table, const Point & p, Array<bool> &result,
		deque<int> *border, MinMax *minmax)
{
	if (arrayAt<FieldType, ftWall>(table, p) != ftStone || result[p])
	{
		if (border != NULL && arrayAt<FieldType, ftWall>(table, p) == ftStone)
			border->push_back(stoneAt[p]);
		return;
	}
	result[p] = true;
	if (minmax != NULL) {
		minmax->minX = std::min(minmax->minX, p.x);
		minmax->maxX = std::max(minmax->maxX, p.x);
		minmax->minY = std::min(minmax->minY, p.y);
		minmax->maxY = std::max(minmax->maxY, p.y);
	}
	floodFillIter(p+p10, result, border, minmax);
	floodFillIter(p+pm10, result, border, minmax);
	floodFillIter(p+p01, result, border, minmax);
	floodFillIter(p+p0m1, result, border, minmax);
}

void floodFill(const Array<FieldType> &table, const Point &p0, Array<bool> &result,
			std::deque<int> *border, MinMax *minmax) {
	result.fill(false);
	if (minmax != NULL) {
		minmax->minX = table.width();
		minmax->maxX = 0;
		minmax->minY = table.height();
		minmax->maxY = 0;
	}
	floodFillIter(table, p0, result, border, minmax);
}
