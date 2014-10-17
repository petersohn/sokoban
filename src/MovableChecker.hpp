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
		calculator_(std::move(calculator))
	{}
	bool check(const Status& status, Point p) const override;
	const char* errorMessage() const override;
};

#endif /* MOVABLECHECKER_H_ */
