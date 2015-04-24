#ifndef SRC_EXTENDEDCORRIDORCHECKER_HPP
#define SRC_EXTENDEDCORRIDORCHECKER_HPP

#include "CorridorCheckerBase.hpp"
#include "HeurCalculator.hpp"
#include "Array.hpp"
#include "Status/floodFill.hpp"
#include "Status/Status.hpp"
#include "Status/floodFill.hpp"
#include "FieldType.hpp"
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
    bool checkCorridorEnding(Point p0, Point side) const
    {
        Point p1 = p0 + side;
        Point pm1 = p0 - side;
        return status.value(p0) != FieldType::wall &&
                status.value(p1) == FieldType::floor &&
                status.value(pm1) == FieldType::floor &&
                (isMovableTo(p1, pm1) || isMovableTo(pm1, p1));
    }

    void floodFill(Point p0, Array<bool>& result, MinMax& minmax)
    {
        Status::BorderType border;
        ::floodFill(status, p0, result, minmax, border);
        simpleStatus.reset(new Status{status.table(),
            State{std::move(std::move(border))}, status.currentPos()});
    }
};

class ExtendedCorridorCheckerStrategyFactory {
    std::shared_ptr<const HeurCalculator> calculator_;

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
