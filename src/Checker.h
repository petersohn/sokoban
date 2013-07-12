#ifndef CHECKER_H_
#define CHECKER_H_

#include <memory>
#include <string>
#include "Dumper/Dumper.h"
#include "Status/Table.h"

class Status;
class Point;

class Checker {
public:
	typedef std::shared_ptr<Checker> Ptr;
	virtual bool check(const Status&, Point ) = 0;
	virtual ~Checker() {}
	// for efficiency reasons, it is assumed that it will
	// return a *real* string constant, or at least
	// remain valid until the next call
	virtual const char* errorMessage() = 0;
};

#endif /* CHECKER_H_ */
