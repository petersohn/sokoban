#include "floodFill.hpp"
#include "FieldType.hpp"
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

			if (status.value(p) == FieldType::floor) {
				result[p] = true;
				pointsToVisit.push_back(p+p10);
				pointsToVisit.push_back(p+pm10);
				pointsToVisit.push_back(p+p01);
				pointsToVisit.push_back(p+p0m1);
			}
		} else {
		}

	}
}

void floodFill(const Status& status, Point p0, Array<bool>& result)
{
	floodFillImpl(status, p0, result, [](Point){});
}

namespace {

class BorderAction {
	const Status& status;
	Status::BorderType& border;
	Array<bool> visitedStones;

public:

	BorderAction(const Status& status, Status::BorderType& border):
		status(status), border(border),
		visitedStones{status.width(), status.height(), false}
	{}

	void operator()(Point p)
	{
		if (status.value(p) == FieldType::stone && !visitedStones[p]) {
			border.push_back(p);
			visitedStones[p] = true;
		}
	}
};

class MinmaxAction {
	const Status& status;
	MinMax& minmax;

public:

	MinmaxAction(const Status& status, MinMax& minmax):
		status(status), minmax(minmax)
	{
		minmax.minX = status.width();
		minmax.maxX = 0;
		minmax.minY = status.height();
		minmax.maxY = 0;
	}

	void operator()(Point p)
	{
		if (status.value(p) == FieldType::floor) {
			minmax.minX = std::min(minmax.minX, p.x);
			minmax.maxX = std::max(minmax.maxX, p.x);
			minmax.minY = std::min(minmax.minY, p.y);
			minmax.maxY = std::max(minmax.maxY, p.y);
		}
	}
};

}

void floodFill(const Status& status, Point p0, Array<bool>& result,
			Status::BorderType& border)
{
	floodFillImpl(status, p0, result, BorderAction{status, border});
}

void floodFill(const Status& status, Point p0, Array<bool>& result,
			MinMax& minmax)
{
	floodFillImpl(status, p0, result, MinmaxAction{status, minmax});
}

void floodFill(const Status& status, Point p0, Array<bool>& result,
			MinMax& minmax, Status::BorderType& border)
{
	BorderAction borderAction{status, border};
	MinmaxAction minmaxAction{status, minmax};

	floodFillImpl(status, p0, result, [&](Point p)
			{
				borderAction(p);
				minmaxAction(p);
			});
}

std::ostream& operator<<(std::ostream& os, const MinMax& minmax)
{
	return os << '(' << minmax.minX << "->" << minmax.maxX << ", " <<
		minmax.minY << "->" << minmax.maxY << ')';
}




