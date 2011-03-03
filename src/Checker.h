#ifndef CHECKER_H_
#define CHECKER_H_

#include <boost/shared_ptr.hpp>

class Status;

class Checker {
public:
	typedef boost::shared_ptr<Checker> Ptr;
	virtual bool check(const Status &) = 0;
	virtual ~Checker() {}
};

#endif /* CHECKER_H_ */
