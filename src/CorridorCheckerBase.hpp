#ifndef CORRIDORCHECKERBASE_HPP
#define CORRIDORCHECKERBASE_HPP

#include "Checker.hpp"
#include "Status/floodFill.hpp"

template <typename StrategyFactory>
class CorridorCheckerBase: public Checker {
	StrategyFactory strategyFactory;
public:
	CorridorCheckerBase(StrategyFactory strategyFactory = StrategyFactory{}): 
			strategyFactory(std::move(strategyFactory)) {}

	bool check(const Status& status, Point p0) const override
	{
		auto strategy = strategyFactory();
		Array<bool> unchecked(3, 3, true);
		Point p1;
		for (p1.x = 0; p1.x < 3; p1.x++)
			for (p1.y = 0; p1.y < 3; p1.y++) {
				Point p = p0 + p1 + Point::pm1m1;
				if (!unchecked[p1] || status.value(p) != FieldType::floor || status.reachable(p))
					continue;
				Array<bool> reach(status.width(), status.height(), false);
				MinMax minmax;
				strategy.floodFill(status, p, reach, minmax);
				if (!reach[status.table().destination()]) {
					if (minmax.minX == minmax.maxX && minmax.minY == minmax.maxY) {
						if (!strategy.checkCorridorEnding(status,
									Point(minmax.minX, minmax.minY - 1), Point::p10) &&
							!strategy.checkCorridorEnding(status,
									Point(minmax.minX, minmax.maxY + 1), Point::p10) &&
							!strategy.checkCorridorEnding(status,
									Point(minmax.minX - 1, minmax.minY), Point::p01) &&
							!strategy.checkCorridorEnding(status,
									Point(minmax.maxX + 1, minmax.minY), Point::p01)) {
							return false;
						}
					} else
					if (minmax.minX == minmax.maxX) {
						if (!strategy.checkCorridorEnding(status,
									Point(minmax.minX, minmax.minY - 1), Point::p10) &&
							!strategy.checkCorridorEnding(status,
									Point(minmax.minX, minmax.maxY + 1), Point::p10)) {
							return false;
						}
					} else
					if (minmax.minY == minmax.maxY) {
						if (!strategy.checkCorridorEnding(status,
									Point(minmax.minX - 1, minmax.minY), Point::p01) &&
							!strategy.checkCorridorEnding(status,
									Point(minmax.maxX + 1, minmax.minY), Point::p01)) {
							return false;
						}
					}
				}
				Point p2;
				for (p2.x = 0; p2.x < 3; p2.x++)
					for (p2.y = 0; p2.y < 3; p2.y++) {
						Point pp = p0 + p2 + Point::pm1m1;
						if (arrayAt<bool>(reach, pp, false))
							unchecked[p2] = false;
					}
			}
		return true;
	}

	const char* errorMessage() const override
	{
		return "corridor found";
	}
};


#endif /* CORRIDORCHECKERBASE_HPP */
