#ifndef CHECKER_HPP
#define CHECKER_HPP

#include "Status/Point.hpp"

#include <string>

namespace sokoban {

class Status;

class Checker {
public:
    virtual bool check(const Status& status, Point p) const = 0;
    virtual ~Checker() {}
    // for efficiency reasons, it is assumed that it will
    // return a *real* string constant, or at least
    // remain valid until the next call
    virtual const char* errorMessage() const = 0;

    template <typename Archive>
    void serialize(Archive& /*ar*/, const unsigned int /*version*/) {
    }
};

} // namespace sokoban

#endif // CHECKER_HPP
