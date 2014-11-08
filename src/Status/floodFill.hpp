#ifndef SRC_STATUS_FLOODFILL_HPP
#define SRC_STATUS_FLOODFILL_HPP

#include "Status.hpp"

struct MinMax {
	int minX, maxX, minY, maxY;
};

void floodFill(const Status& status, Point p0, Array<bool>& result);
void floodFill(const Status& status, Point p0, Array<bool>& result,
			Status::BorderType& border);
void floodFill(const Status& status, Point p0, Array<bool>& result,
			MinMax& minmax);

#endif /* SRC_STATUS_FLOODFILL_HPP */
