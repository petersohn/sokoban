#ifndef SRC_MOVABLECHECKERIMPL_HPP
#define SRC_MOVABLECHECKERIMPL_HPP

#include "Status/Status.hpp"
#include "HeurCalculator/HeurCalculator.hpp"
#include "FieldType.hpp"
#include <boost/optional.hpp>
#include <unordered_set>
#include <algorithm>

namespace sokoban {

class MovableCheckerImpl {
    const Status& status_;
    boost::optional<Status> simpleStatus_;
    const HeurCalculator& calculator_;
    std::unordered_set<Point> calculated_;

    bool isMovableTo(Point from, Point to)
    {
        if (calculator_.calculateStone(status_, to) < 0) {
            return false;
        }

        if (status_.value(from) == FieldType::floor) {
            return simpleStatus_ ? simpleStatus_->reachable(from) : true;
        }

        return true;
    }

    bool isValid(Point p)
    {
        return status_.value(p) == FieldType::floor || stoneMovable(p);
    }

    bool isMovable(Point p, Point d)
    {
        Point pd = p+d;
        Point pmd = p-d;
        if (status_.value(pd) == FieldType::wall ||
                status_.value(pmd) == FieldType::wall) {
            return false;
        }

        std::vector<Point> reachableCheck;
        if (!isMovableTo(pmd, pd) && !isMovableTo(pd, pmd)) {
            return false;
        }

        if (!isValid(pd) || !isValid(pmd)) {
            return false;
        }

        return true;
    }

public:
    MovableCheckerImpl(const Status& status, const HeurCalculator& calculator):
        status_(status),
        calculator_(calculator)
    {}

    bool stoneMovable(Point p)
    {
        if (calculated_.count(p) != 0) {
            return false;
        }

        calculated_.insert(p);
        return isMovable(p, p10) || isMovable(p, p01);
    }

    bool checkReachability(Point startingPoint)
    {
        simpleStatus_ = Status{status_.table(),
            State(std::move(calculated_)), status_.currentPos()};
        calculated_.clear();
        return stoneMovable(startingPoint);
    }
};

} // namespace sokoban

#endif /* SRC_MOVABLECHECKERIMPL_HPP */
