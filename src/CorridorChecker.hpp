#ifndef CORRIDORCHECKER_H_
#define CORRIDORCHECKER_H_

#include "CorridorCheckerBase.hpp"
#include "HeurCalculator.hpp"
#include "Array.hpp"
#include "Status/floodFill.hpp"

class CorridorCheckerStrategy {
	const HeurCalculator* calculator_;
	const Status& status;
public:
	CorridorCheckerStrategy(const HeurCalculator* calculator,
			const Status& status):
		calculator_(calculator), status(status)
	{}
	bool checkCorridorEnding(Point p0, Point side) const;
	void floodFill(Point p0, Array<bool>& result, MinMax& minmax);
};

class CorridorCheckerStrategyFactory {
	HeurCalculator::Ptr calculator_;

public:
	CorridorCheckerStrategyFactory(std::shared_ptr<const HeurCalculator> heurCalculator):
		calculator_(std::move(heurCalculator))
	{}

	CorridorCheckerStrategy operator()(const Status& status) const
	{
		return CorridorCheckerStrategy{calculator_.get(), status};
	}
};

using CorridorChecker = CorridorCheckerBase<CorridorCheckerStrategyFactory>;

#endif /* CORRIDORCHECKER_H_ */
