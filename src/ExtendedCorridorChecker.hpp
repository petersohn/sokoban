#ifndef SRC_EXTENDEDCORRIDORCHECKER_HPP
#define SRC_EXTENDEDCORRIDORCHECKER_HPP

#include "CorridorCheckerBase.hpp"
#include "HeurCalculator.hpp"
#include "Array.hpp"
#include "Status/floodFill.hpp"
#include <memory>

class ExtendedCorridorCheckerStrategy {
	const HeurCalculator* calculator_;
	std::unique_ptr<Status> simpleStatus;
	bool isMovableTo(const Status& status, Point from, Point to) const
	{
		return simpleStatus->reachable(from) &&
			calculator_->calculateStone(status, to) > 0;
	}
public:
	ExtendedCorridorCheckerStrategy(const HeurCalculator* calculator):
		calculator_(calculator)
	{}
	bool checkCorridorEnding(const Status &status, Point p0, Point side) const;
	void floodFill(const Status& status, Point p0, Array<bool>& result,
				MinMax& minmax);
};

class ExtendedCorridorCheckerStrategyFactory {
	HeurCalculator::Ptr calculator_;

public:
	ExtendedCorridorCheckerStrategyFactory(std::shared_ptr<const HeurCalculator> heurCalculator):
		calculator_(std::move(heurCalculator))
	{}

	ExtendedCorridorCheckerStrategy operator()() const
	{
		return ExtendedCorridorCheckerStrategy{calculator_.get()};
	}
};

using ExtendedCorridorChecker = CorridorCheckerBase<ExtendedCorridorCheckerStrategyFactory>;

#endif /* SRC_EXTENDEDCORRIDORCHECKER_HPP */
