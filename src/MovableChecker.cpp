#include "MovableChecker.hpp"
#include "MovableCheckerImpl.hpp"

bool MovableChecker::check(const Status& status, Point p) const {
    MovableCheckerImpl ch(status, *calculator_);
    return ch.stoneMovable(p);
}

const char* MovableChecker::errorMessage() const {
    return "not movable";
}

