#include "floodFill.hpp"
#include <vector>

template <typename Action>
void floodFillImpl(const Status &status, Point p0, Array<bool> &result,
		Action action)
{
	result.fill(false);

	std::vector<Point> pointsToVisit;
	pointsToVisit.reserve(status.width()*status.height());
	pointsToVisit.push_back(p0);

	while (!pointsToVisit.empty()) {
		Point p = pointsToVisit.back();
		pointsToVisit.pop_back();

		if (!arrayAt(result, p, true)) {
			action(p);

			if (status.value(p) == ftFloor) {
				result[p] = true;
				pointsToVisit.push_back(p+Point::p10);
				pointsToVisit.push_back(p+Point::pm10);
				pointsToVisit.push_back(p+Point::p01);
				pointsToVisit.push_back(p+Point::p0m1);
			}
		} else {
		}

	}
}

void floodFill(const Status& status, Point p0, Array<bool>& result)
{
	floodFillImpl(status, p0, result, [](Point){});
}

void floodFill(const Status& status, Point p0, Array<bool>& result,
			Status::BorderType& border)
{
	Array<bool> visitedStones{status.width(), status.height(), false};

	floodFillImpl(status, p0, result, [&](Point p)
		{
			if (status.value(p) == ftStone && !visitedStones[p]) {
				border.push_back(p);
				visitedStones[p] = true;
			}
		});
}

void floodFill(const Status& status, Point p0, Array<bool>& result,
			MinMax& minmax)
{
	minmax.minX = status.width();
	minmax.maxX = 0;
	minmax.minY = status.height();
	minmax.maxY = 0;

	floodFillImpl(status, p0, result, [&](Point p)
		{
			if (status.value(p) == ftFloor) {
				minmax.minX = std::min(minmax.minX, p.x);
				minmax.maxX = std::max(minmax.maxX, p.x);
				minmax.minY = std::min(minmax.minY, p.y);
				minmax.maxY = std::max(minmax.maxY, p.y);
			}
		});
}

