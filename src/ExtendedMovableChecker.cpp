#include "ExtendedMovableChecker.hpp"
#include "FieldType.hpp"
#include <unordered_set>
#include <algorithm>

namespace {

class InternalChecker {
	const Status& status_;
	boost::optional<Status> simpleStatus_;
	const HeurCalculator& calculator_;
	std::unordered_set<Point> calculated_;

	bool isMovableTo(Point from, Point to)
	{
		if (calculator_.calculateStone(status_, to) < 0) {
			return false;
		}

		if (status_.value(from) == FieldType::floor) {
			return simpleStatus_ ? simpleStatus_->reachable(from) : true;
		}

		return true;
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

		std::vector<Point> reachableCheck;
		if (!isMovableTo(pmd, pd) && !isMovableTo(pd, pmd)) {
			return false;
		}

		if (!isValid(pd) || !isValid(pmd)) {
			return false;
		}

		return true;
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

	bool checkReachability(Point startingPoint)
	{
		simpleStatus_ = Status{status_.table(),
			State(std::move(calculated_)), status_.currentPos()};
		calculated_.clear();
		return stoneMovable(startingPoint);
	}
};

}

bool ExtendedMovableChecker::check(const Status& status, Point p) const {
	InternalChecker ch(status, *calculator_);
	return ch.stoneMovable(p) && ch.checkReachability(p);
}

const char* ExtendedMovableChecker::errorMessage() const {
	return "not movable";
}

