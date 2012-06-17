#ifndef HEURCALCULATOR_H_
#define HEURCALCULATOR_H_

#include <memory>
#include <boost/noncopyable.hpp>

class Point;
class Status;

class HeurCalculator: public boost::noncopyable {
public:
	typedef std::shared_ptr<HeurCalculator> Ptr;
	virtual ~HeurCalculator() {}
	virtual int calculateStone(const Status& status, const Point& p) = 0;
	virtual int calculateStatus(const Status& status) = 0;
};

#endif /* HEURCALCULATOR_H_ */
