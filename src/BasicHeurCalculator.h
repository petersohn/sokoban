#ifndef BASICHEURCALCULATOR_H_
#define BASICHEURCALCULATOR_H_

#include "ProblemHeurCalculator.h"
#include "Array.h"

class BasicHeurCalculator: public ProblemHeurCalculator {
	Array<int> distances_;
	virtual int doCalculate(const Status &status) const;
	virtual void init();

	bool checkDistance(const Point & p, const Point & d, int dist);
public:
	BasicHeurCalculator() {}
};

#endif /* BASICHEURCALCULATOR_H_ */
