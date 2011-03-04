#include "CorridorChecker.h"


bool CorridorChecker::check(const Status & status, const Point & p0)
{
	bool kell[3][3];
	for (int x = 0; x < 3; x++)
		for (int y = 0; y < 3; y++)
			kell[x][y] = true;

	for (int x = 0; x < 3; x++)
		for (int y = 0; y < 3; y++) {
			Point p = p0 + Point(x - 1, y - 1);
			if (!kell[x][y] || status.value(p) != ftFloor || status.reachable(p))
				continue;
			Array<bool> reach(status.width(), status.height(), false);
			MinMax minmax;
			floodFill(status, p, reach, NULL, &minmax);
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
//						cerr << "Bing!" << endl;
						if (enableDump_) {
							dumpStatus(dumpFile_, status, "1x1 corridor found", &reach);
						}
						return false;
					}
				} else
				if (minmax.minX == minmax.maxX) {
					if (!checkCorridorEnding(status,
								Point(minmax.minX, minmax.minY - 1), Point::p10) &&
						!checkCorridorEnding(status,
								Point(minmax.minX, minmax.maxY + 1), Point::p10)) {
						if (enableDump_) {
							dumpStatus(dumpFile_, status, "Vertical corridor found", &reach);
						}
						return false;
					}
				} else
				if (minmax.minY == minmax.maxY) {
					if (!checkCorridorEnding(status,
								Point(minmax.minX - 1, minmax.minY), Point::p01) &&
						!checkCorridorEnding(status,
								Point(minmax.maxX + 1, minmax.minY), Point::p01)) {
						if (enableDump_)
							dumpStatus(dumpFile_, status, "Horizontal corridor found", &reach);
						return false;
					}
				}
			}
			for (int xx = 0; xx < 3; xx++)
				for (int yy = 0; yy < 3; yy++) {
					Point pp = p0 + Point(xx - 1, yy - 1);
					if (arrayAt<bool>(reach, pp, false))
						kell[xx][yy] = false;
				}
		}
	return true;
}

bool CorridorChecker::checkCorridorEnding(const Status & status,
			const Point & p0, const Point & side)
{
	Point p1 = p0 + side;
	Point pm1 = p0 - side;
	return status.value(p0) != ftWall &&
			status.value(p1) == ftFloor &&
			status.value(pm1) == ftFloor &&
			(calculator_->calculateStone(status, p1) || calculator_->calculateStone(status, pm1));
}

