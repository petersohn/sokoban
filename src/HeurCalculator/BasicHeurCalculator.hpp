#ifndef HEURCALCULATOR_BASICHEURCALCULATOR_HPP
#define HEURCALCULATOR_BASICHEURCALCULATOR_HPP

#include "DistanceCalculator.hpp"
#include "Matrix.hpp"
#include "HeurCalculator/TableHeurCalculator.hpp"

namespace sokoban {

class BasicStoneCalculator {
    Matrix<int> distances_;

public:
    BasicStoneCalculator() = default;
    BasicStoneCalculator(const Table& table) {
        distances_ = calculateDistances(table.width(), table.height(),
                table.destination(),
                [&table](Point p, Point d) {
                    return !table.wall(p+d) && !table.wall(p+d*2);
                });
    }

    BasicStoneCalculator(const BasicStoneCalculator&) = default;
    BasicStoneCalculator& operator=(const BasicStoneCalculator&) = default;
    BasicStoneCalculator(BasicStoneCalculator&&) = default;
    BasicStoneCalculator& operator=(BasicStoneCalculator&&) = default;

    float operator()(const Status&, Point p) const {
        return distances_[p];
    }

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int /*version*/)
    {
        ar & distances_;
    }
};

using BasicHeurCalculator = TableHeurCalculator<BasicStoneCalculator>;

} // namespace sokoban

#endif // HEURCALCULATOR_BASICHEURCALCULATOR_HPP
