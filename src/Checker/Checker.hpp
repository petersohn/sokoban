#ifndef CHECKER_HPP
#define CHECKER_HPP

#include <string>

class Status;
class Point;

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

#endif // CHECKER_HPP
