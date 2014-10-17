#ifndef CORRIDORCHECKER_H_
#define CORRIDORCHECKER_H_

#include "Checker.hpp"
#include "HeurCalculator.hpp"

class CorridorChecker: public Checker {
	HeurCalculator::Ptr calculator_;
	bool checkCorridorEnding(const Status &status,
		Point p0, Point side) const;
public:
	CorridorChecker(HeurCalculator::Ptr calculator):
		calculator_(std::move(calculator))
	{}
	bool check(const Status& status, Point p0) const override;
	const char* errorMessage() const override;
};

#endif /* CORRIDORCHECKER_H_ */
