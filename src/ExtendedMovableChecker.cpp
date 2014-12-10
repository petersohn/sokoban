#include "ExtendedMovableChecker.hpp"
#include "FieldType.hpp"
#include "MovableCheckerImpl.hpp"
#include <unordered_set>
#include <algorithm>

bool ExtendedMovableChecker::check(const Status& status, Point p) const {
	MovableCheckerImpl ch(status, *calculator_);
	return ch.stoneMovable(p) && ch.checkReachability(p);
}

const char* ExtendedMovableChecker::errorMessage() const {
	return "not movable";
}

