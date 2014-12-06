#ifndef SRC_EXTENDEDCORRIDORCHECKER_HPP
#define SRC_EXTENDEDCORRIDORCHECKER_HPP

#include "Checker.hpp"
#include "HeurCalculator.hpp"

class ExtendedCorridorChecker: public Checker {
	HeurCalculator::Ptr calculator_;
	bool checkCorridorEnding(const Status &status,
		const Array<bool>& reachability, Point p0, Point side) const;
public:
	ExtendedCorridorChecker(HeurCalculator::Ptr calculator):
		calculator_(std::move(calculator))
	{}
	bool check(const Status& status, Point p0) const override;
	const char* errorMessage() const override;
};

#endif /* SRC_EXTENDEDCORRIDORCHECKER_HPP */
