#ifndef MOVABLECHECKER_H_
#define MOVABLECHECKER_H_

#include "Checker.hpp"
#include "Status/Status.hpp"
#include "Status/Point.hpp"
#include "HeurCalculator.hpp"

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
