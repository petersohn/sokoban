#ifndef SRC_EXTENDEDMOVABLECHECKER_HPP
#define SRC_EXTENDEDMOVABLECHECKER_HPP

#include "Checker.hpp"
#include "Status/Point.hpp"
#include "HeurCalculator.hpp"

class Status;

class ExtendedMovableChecker: public Checker {
	HeurCalculator::Ptr calculator_;
public:
	ExtendedMovableChecker(HeurCalculator::Ptr calculator):
		calculator_(std::move(calculator))
	{}
	bool check(const Status& status, Point p) const override;
	const char* errorMessage() const override;
};

#endif /* SRC_EXTENDEDMOVABLECHECKER_HPP */
