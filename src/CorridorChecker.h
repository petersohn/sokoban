#ifndef CORRIDORCHECKER_H_
#define CORRIDORCHECKER_H_

#include "Checker.h"
#include "HeurCalculator.h"

class CorridorChecker: public Checker {
	HeurCalculator::Ptr calculator_;
	bool checkCorridorEnding(const Status &status,
		const Point &p0, const Point &side);
public:
	CorridorChecker(HeurCalculator::Ptr calculator):
		calculator_(calculator)
	{}
	virtual bool check(const Status &status, const Point &p0);
};

#endif /* CORRIDORCHECKER_H_ */