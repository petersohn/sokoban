#include "MovableChecker.h"
#include <boost/unordered_set.hpp>

class InternalChecker {
	HeurCalculator::Ptr calculator_;
	boost::unordered_set<Point> volt_;
	bool isMovable(const Status &status, const Point & p,
		int &count);
public:
	InternalChecker(HeurCalculator::Ptr calculator):
		calculator_(calculator)
	{}
	bool stoneMovable(const Status &status, const Point &p);
};

bool MovableChecker::check(const Status &status, const Point &p) {
	InternalChecker ch(calculator_);
	return ch.stoneMovable(status, p);
}

bool InternalChecker::stoneMovable(const Status &status, const Point &p) {
	volt_.insert(p);
	int count = 0;
	if (isMovable(status, p+Point::p10, count) &&
			isMovable(status, p+Point::pm10, count) && count > 0)
		return true;
	count = 0;
	return (isMovable(status, p+Point::p01, count) &&
			isMovable(status, p+Point::p0m1, count) && count > 0);
}

bool InternalChecker::isMovable(const Status &status, const Point & p,
		int &count) {
	if (status.value(p) == ftFloor)
	{
		if (calculator_->calculateStone(status, p) >= 0)
			++count;
		return true;
	}
	if (status.value(p) == ftWall)
		return false;
	if (volt_.count(p) != 0)
		return false;
	return stoneMovable(status, p);
}

