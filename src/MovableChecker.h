#ifndef MOVABLECHECKER_H_
#define MOVABLECHECKER_H_

#include "Checker.h"
#include "Status/Status.h"
#include "Status/Point.h"
#include "HeurCalculator.h"

class MovableChecker: public Checker {
	HeurCalculator::Ptr calculator_;
public:
	MovableChecker(HeurCalculator::Ptr calculator):
		calculator_(calculator)
	{}
	virtual bool check(const Status &status, Point p);
	virtual const char* errorMessage();
};

#endif /* MOVABLECHECKER_H_ */
