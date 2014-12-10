#ifndef SRC_EXTENDEDCORRIDORCHECKER_HPP
#define SRC_EXTENDEDCORRIDORCHECKER_HPP

#include "CorridorCheckerBase.hpp"
#include "HeurCalculator.hpp"
#include "Array.hpp"
#include "Status/floodFill.hpp"
#include <memory>

class ExtendedCorridorCheckerStrategy {
	const HeurCalculator* calculator_;
	const Status& status;
	std::unique_ptr<Status> simpleStatus;
	bool isMovableTo(Point from, Point to) const
	{
		return simpleStatus->reachable(from) &&
			calculator_->calculateStone(status, to) > 0;
	}
public:
	ExtendedCorridorCheckerStrategy(const HeurCalculator* calculator,
			const Status& status):
		calculator_(calculator), status(status)
	{}
	bool checkCorridorEnding(Point p0, Point side) const;
	void floodFill(Point p0, Array<bool>& result, MinMax& minmax);
};

class ExtendedCorridorCheckerStrategyFactory {
	HeurCalculator::Ptr calculator_;

public:
	ExtendedCorridorCheckerStrategyFactory(std::shared_ptr<const HeurCalculator> heurCalculator):
		calculator_(std::move(heurCalculator))
	{}

	ExtendedCorridorCheckerStrategy operator()(const Status& status) const
	{
		return ExtendedCorridorCheckerStrategy{calculator_.get(), status};
	}
};

using ExtendedCorridorChecker = CorridorCheckerBase<ExtendedCorridorCheckerStrategyFactory>;

#endif /* SRC_EXTENDEDCORRIDORCHECKER_HPP */
