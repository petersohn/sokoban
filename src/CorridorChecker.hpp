#ifndef CORRIDORCHECKER_H_
#define CORRIDORCHECKER_H_

#include "Checker.hpp"
#include "HeurCalculator.hpp"

class CorridorChecker: public Checker {
	HeurCalculator::Ptr calculator_;
	bool checkCorridorEnding(const Status &status,
		Point p0, Point side);
public:
	CorridorChecker(HeurCalculator::Ptr calculator):
		calculator_(calculator)
	{}
	virtual bool check(const Status &status, Point p0);
	virtual const char* errorMessage();
};

#endif /* CORRIDORCHECKER_H_ */
