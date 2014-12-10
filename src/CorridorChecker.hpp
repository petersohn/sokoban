#ifndef CORRIDORCHECKER_H_
#define CORRIDORCHECKER_H_

#include "CorridorCheckerBase.hpp"
#include "HeurCalculator.hpp"
#include "Array.hpp"
#include "Status/floodFill.hpp"

class CorridorCheckerStrategy {
	const HeurCalculator* calculator_;
public:
	CorridorCheckerStrategy(const HeurCalculator* calculator):
		calculator_(calculator)
	{}
	bool checkCorridorEnding(const Status &status,
		Point p0, Point side) const;
	void floodFill(const Status& status, Point p0, Array<bool>& result,
				MinMax& minmax);
};

class CorridorCheckerStrategyFactory {
	HeurCalculator::Ptr calculator_;

public:
	CorridorCheckerStrategyFactory(std::shared_ptr<const HeurCalculator> heurCalculator):
		calculator_(std::move(heurCalculator))
	{}

	CorridorCheckerStrategy operator()() const
	{
		return CorridorCheckerStrategy{calculator_.get()};
	}
};

using CorridorChecker = CorridorCheckerBase<CorridorCheckerStrategyFactory>;

#endif /* CORRIDORCHECKER_H_ */
