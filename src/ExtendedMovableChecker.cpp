#include "ExtendedMovableChecker.hpp"
#include "FieldType.hpp"
#include <unordered_set>

class InternalChecker {
	const Status& status_;
	const HeurCalculator& calculator_;
	std::unordered_set<Point> calculated_;

	bool isMovableTo(Point from, Point to)
	{
		return calculator_.calculateStone(status_, to) >= 0 &&
				(status_.reachable(from) ||
				 status_.value(from) == FieldType::stone);
	}

	bool isValid(Point p)
	{
		return status_.value(p) == FieldType::floor || stoneMovable(p);
	}

	bool isMovable(Point p, Point d)
	{
		Point pd = p+d;
		Point pmd = p-d;
		if (status_.value(pd) == FieldType::wall ||
				status_.value(pmd) == FieldType::wall) {
			return false;
		}

		if (!isMovableTo(pd, pmd) && !isMovableTo(pmd, pd)) {
			return false;
		}

		return isValid(pd) && isValid(pmd);
	}

public:
	InternalChecker(const Status& status, const HeurCalculator& calculator):
		status_(status),
		calculator_(calculator)
	{}

	bool stoneMovable(Point p)
	{
		if (calculated_.count(p) != 0) {
			return false;
		}

		calculated_.insert(p);
		return isMovable(p, Point::p10) || isMovable(p, Point::p01);
	}

};

bool ExtendedMovableChecker::check(const Status& status, Point p) const {
	InternalChecker ch(status, *calculator_);
	return ch.stoneMovable(p);
}

const char* ExtendedMovableChecker::errorMessage() const {
	return "not movable";
}

