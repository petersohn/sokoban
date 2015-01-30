#ifndef CHECKER_H_
#define CHECKER_H_

#include <memory>
#include <string>
#include "Status/Table.hpp"

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

#endif /* CHECKER_H_ */
