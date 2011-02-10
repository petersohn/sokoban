#ifndef BASICHEURCALCULATOR_H_
#define BASICHEURCALCULATOR_H_

#include "StoneBasedHeurCalculator.h"
#include "Array.h"

class BasicHeurCalculator: public TableHeurCalculator {
	Array<int> distances_;
	virtual void init();
	virtual int doCalculateStone(const Status &status, const Point &p) const;
	bool checkDistance(const Point & p, const Point & d, int dist);
public:
	BasicHeurCalculator() {}
};

#endif /* BASICHEURCALCULATOR_H_ */
