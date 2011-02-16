#ifndef HEURCALCULATOR_H_
#define HEURCALCULATOR_H_

#include <boost/shared_ptr.hpp>

class Point;
class Status;

class HeurCalculator {
public:
	typedef boost::shared_ptr<HeurCalculator> Ptr;
	virtual ~HeurCalculator() {}
	virtual int calculateStone(const Status &, const Point &) = 0;
	virtual int calculateStatus(const Status &) const = 0;
	static Ptr create();
};

#endif /* HEURCALCULATOR_H_ */
