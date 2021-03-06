#include "Checker/MovableChecker.hpp"
#include "Checker/MovableCheckerImpl.hpp"

namespace sokoban {

bool MovableChecker::check(const Status& status, Point p) const {
    MovableCheckerImpl ch(status, *calculator_);
    return ch.stoneMovable(p);
}

const char* MovableChecker::errorMessage() const {
    return "not movable";
}

} // namespace sokoban
