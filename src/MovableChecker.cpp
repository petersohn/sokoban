#include "MovableChecker.hpp"
#include "FieldType.hpp"
#include <unordered_set>

class InternalChecker {
	const Status& status_;
	const HeurCalculator& calculator_;
	std::unordered_set<Point> calculated_;
	bool isMovable(Point p, int &count);
public:
	InternalChecker(const Status& status, const HeurCalculator& calculator):
		status_(status),
		calculator_(calculator)
	{}
	bool stoneMovable(Point p);
};

bool InternalChecker::stoneMovable(Point p) {
	calculated_.insert(p);
	int count = 0;
	if (isMovable(p+Point::p10, count) &&
			isMovable(p+Point::pm10, count) && count > 0)
		return true;
	count = 0;
	return (isMovable(p+Point::p01, count) &&
			isMovable(p+Point::p0m1, count) && count > 0);
}

bool InternalChecker::isMovable(Point p, int &count) {
	if (status_.value(p) == FieldType::wall)
		return false;
	if (calculator_.calculateStone(status_, p) >= 0)
			++count;
	if (status_.value(p) == FieldType::floor)
		return true;
	if (calculated_.count(p) != 0)
		return false;
	return stoneMovable(p);
}



bool MovableChecker::check(const Status& status, Point p) const {
	InternalChecker ch(status, *calculator_);
	return ch.stoneMovable(p);
}

const char* MovableChecker::errorMessage() const {
	return "not movable";
}

