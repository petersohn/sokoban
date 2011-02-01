#ifndef TABLEHEURCALCULATOR_H_
#define TABLEHEURCALCULATOR_H_

#include "HeurCalculator.h"
#include "Array.h"
#include <boost/scoped_ptr.hpp>

class Problem;

class TableHeurCalculator: public HeurCalculator {
	boost::scoped_ptr<Array<int> > table_;

	virtual int stoneValue(const Point &) = 0;
public:
	TableHeurCalculator();

	void initialize(const Problem &);
	virtual int calculate(const Status &);
};

#endif /* TABLEHEURCALCULATOR_H_ */
