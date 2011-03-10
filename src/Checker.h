#ifndef CHECKER_H_
#define CHECKER_H_

#include <boost/shared_ptr.hpp>
#include <string>
#include "Dumper.h"

class Status;
class Point;

class Checker {
public:
	typedef boost::shared_ptr<Checker> Ptr;
	virtual bool check(const Status&, const Point&) = 0;
	virtual ~Checker() {}
	// for efficiency reasons, it is assumed that it will
	// return a *real* string constant, or at least
	// remain valid until the next call
	virtual const char* errorMessage() = 0;
};

#endif /* CHECKER_H_ */
