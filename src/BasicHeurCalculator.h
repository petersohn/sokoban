#ifndef BASICHEURCALCULATOR_H_
#define BASICHEURCALCULATOR_H_

#include "TableHeurCalculator.h"
#include "Array.h"

class BasicHeurCalculator: public TableHeurCalculator {
	Array<int> distances_;
	virtual void init();
	virtual int doCalculateStone(const Status &status, Point p);
	bool checkDistance(Point  p, Point  d, int dist);
public:
	BasicHeurCalculator() {}
};

#endif /* BASICHEURCALCULATOR_H_ */
