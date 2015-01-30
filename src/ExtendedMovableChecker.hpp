#ifndef SRC_EXTENDEDMOVABLECHECKER_HPP
#define SRC_EXTENDEDMOVABLECHECKER_HPP

#include "Checker.hpp"
#include <memory>

class Status;
class HeurCalculator;
class Point;

class ExtendedMovableChecker: public Checker {
	std::shared_ptr<const HeurCalculator> calculator_;
public:
	ExtendedMovableChecker(std::shared_ptr<const HeurCalculator> calculator):
		calculator_(std::move(calculator))
	{}
	bool check(const Status& status, Point p) const override;
	const char* errorMessage() const override;
};

#endif /* SRC_EXTENDEDMOVABLECHECKER_HPP */
