#include "MovableChecker.hpp"
#include "FieldType.hpp"
#include <unordered_set>

class InternalChecker {
	const HeurCalculator& calculator_;
	std::unordered_set<Point> calculated_;
	bool isMovable(const Status& status, Point  p,
		int &count);
public:
	InternalChecker(const HeurCalculator& calculator):
		calculator_(calculator)
	{}
	bool stoneMovable(const Status& status, Point p);
};

bool InternalChecker::stoneMovable(const Status& status, Point p) {
	calculated_.insert(p);
	int count = 0;
	if (isMovable(status, p+Point::p10, count) &&
			isMovable(status, p+Point::pm10, count) && count > 0)
		return true;
	count = 0;
	return (isMovable(status, p+Point::p01, count) &&
			isMovable(status, p+Point::p0m1, count) && count > 0);
}

bool InternalChecker::isMovable(const Status& status, Point  p,
		int &count) {
	if (status.value(p) == FieldType::wall)
		return false;
	if (calculator_.calculateStone(status, p) >= 0)
			++count;
	if (status.value(p) == FieldType::floor)
		return true;
	if (calculated_.count(p) != 0)
		return false;
	return stoneMovable(status, p);
}



bool MovableChecker::check(const Status& status, Point p) const {
	InternalChecker ch(*calculator_);
	return ch.stoneMovable(status, p);
}

const char* MovableChecker::errorMessage() const {
	return "not movable";
}

