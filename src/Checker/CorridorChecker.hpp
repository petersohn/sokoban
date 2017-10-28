#ifndef CORRIDORCHECKER_H_
#define CORRIDORCHECKER_H_

#include "Checker/CorridorCheckerBase.hpp"
#include "HeurCalculator/HeurCalculator.hpp"
#include "Array.hpp"
#include "Status/Status.hpp"
#include "Status/floodFill.hpp"
#include "FieldType.hpp"

#include <boost/serialization/shared_ptr.hpp>

namespace sokoban {

class CorridorCheckerStrategy {
    const HeurCalculator* calculator_;
    const Status& status;
public:
    CorridorCheckerStrategy(const HeurCalculator* calculator,
            const Status& status):
        calculator_(calculator), status(status)
    {}

    bool checkCorridorEnding(Point p0, Point side) const
    {
        Point p1 = p0 + side;
        Point pm1 = p0 - side;
        return status.value(p0) != FieldType::wall &&
                status.value(p1) == FieldType::floor &&
                status.value(pm1) == FieldType::floor &&
                (calculator_->calculateStone(status, p1) >= 0 ||
                 calculator_->calculateStone(status, pm1) >= 0);
    }

    void floodFill(Point p0, Array<bool>& result, MinMax& minmax)
    {
        sokoban::floodFill(status, p0, result, minmax);
    }
};

class CorridorCheckerStrategyFactory {
    std::shared_ptr<HeurCalculator> calculator_;

public:
    CorridorCheckerStrategyFactory() = default;

    CorridorCheckerStrategyFactory(
            std::shared_ptr<HeurCalculator> heurCalculator):
        calculator_(std::move(heurCalculator))
    {}

    CorridorCheckerStrategy operator()(const Status& status) const
    {
        return CorridorCheckerStrategy{calculator_.get(), status};
    }

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int /*version*/)
    {
        ar & calculator_;
    }
};

using CorridorChecker = CorridorCheckerBase<CorridorCheckerStrategyFactory>;

} // namespace sokoban

#endif /* CORRIDORCHECKER_H_ */
