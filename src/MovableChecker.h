#ifndef MOVABLECHECKER_H_
#define MOVABLECHECKER_H_

#include "Checker.h"
#include "Status.h"
#include "Point.h"
#include "HeurCalculator.h"

class MovableChecker: public Checker {
	HeurCalculator::Ptr calculator_;
public:
	MovableChecker(HeurCalculator::Ptr calculator):
		calculator_(calculator)
	{}
	virtual bool check(const Status &status, const Point &p);
	virtual const char* errorMessage();
};

#endif /* MOVABLECHECKER_H_ */
