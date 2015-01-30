#ifndef SRC_CHECKER_HPP
#define SRC_CHECKER_HPP

#include <string>

class Status;
class Point;

class Checker {
public:
	virtual bool check(const Status&, Point ) const = 0;
	virtual ~Checker() {}
	// for efficiency reasons, it is assumed that it will
	// return a *real* string constant, or at least
	// remain valid until the next call
	virtual const char* errorMessage() const = 0;
};

#endif /* SRC_CHECKER_HPP */
