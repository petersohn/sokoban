#include "Checker/ExtendedMovableChecker.hpp"
#include "Checker/MovableCheckerImpl.hpp"

namespace sokoban {

bool ExtendedMovableChecker::check(const Status& status, Point p) const {
    MovableCheckerImpl ch(status, *calculator_);
    return ch.stoneMovable(p) && ch.checkReachability(p);
}

const char* ExtendedMovableChecker::errorMessage() const {
    return "not movable";
}

} // namespace sokoban
