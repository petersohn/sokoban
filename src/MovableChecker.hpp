#ifndef MOVABLECHECKER_H_
#define MOVABLECHECKER_H_

#include "Checker.hpp"
#include <memory>

class Status;
class HeurCalculator;
class Point;

class MovableChecker: public Checker {
	std::shared_ptr<const HeurCalculator> calculator_;
public:
	MovableChecker(std::shared_ptr<const HeurCalculator> calculator):
		calculator_(std::move(calculator))
	{}
	bool check(const Status& status, Point p) const override;
	const char* errorMessage() const override;
};

#endif /* MOVABLECHECKER_H_ */
