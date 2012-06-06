#ifndef HEURCALCULATOR_H_
#define HEURCALCULATOR_H_

#include <memory>

class Point;
class Status;

class HeurCalculator {
public:
	typedef std::shared_ptr<HeurCalculator> Ptr;
	virtual ~HeurCalculator() {}
	virtual int calculateStone(const Status &, const Point &) = 0;
	virtual int calculateStatus(const Status &) = 0;
};

#endif /* HEURCALCULATOR_H_ */
