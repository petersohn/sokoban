#include "ExtendedMovableChecker.hpp"
#include "FieldType.hpp"
#include <unordered_set>
#include <algorithm>

namespace {

class InternalChecker {
	const Status& status_;
	const HeurCalculator& calculator_;
	std::unordered_set<Point> calculated_;
	std::vector<std::vector<Point>> reachableChecks;

	bool isMovableTo(Point from, Point to, std::vector<Point>& reachableCheck)
	{
		if (calculator_.calculateStone(status_, to) < 0) {
			return false;
		}

		if (status_.value(from) == FieldType::floor) {
			reachableCheck.push_back(from);
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
		bool movable1 = isMovableTo(pd, pmd, reachableCheck);
		bool movable2 = isMovableTo(pmd, pd, reachableCheck);
		if (!movable1 && !movable2) {
			return false;
		}

		if (!isValid(pd) || !isValid(pmd)) {
			return false;
		}
		if (!reachableCheck.empty()) {
			reachableChecks.push_back(std::move(reachableCheck));
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

	bool checkReachability()
	{
		Status simpleStatus{status_.table()};

		for (Point p: calculated_) {
			simpleStatus.addStone(p);
		}

		simpleStatus.currentPos(status_.currentPos());

		auto result = std::all_of(reachableChecks.begin(), reachableChecks.end(),
				[&](const std::vector<Point>& v)
				{
					auto result = std::any_of(v.begin(), v.end(),
						[&](Point p) {
							auto result = simpleStatus.reachable(p);
							return result;
						});
					return result;
				});
		return result;
	}
};

}

bool ExtendedMovableChecker::check(const Status& status, Point p) const {
	InternalChecker ch(status, *calculator_);
	return ch.stoneMovable(p) && ch.checkReachability();
}

const char* ExtendedMovableChecker::errorMessage() const {
	return "not movable";
}

