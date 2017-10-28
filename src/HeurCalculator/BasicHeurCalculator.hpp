#ifndef BASICHEURCALCULATOR_H_
#define BASICHEURCALCULATOR_H_

#include "HeurCalculator/TableHeurCalculator.hpp"
#include "Array.hpp"

namespace sokoban {

class BasicStoneCalculator {
    Array<float> distances_;
    bool checkDistance(const Table& table, Point  p, Point  d, int dist);
public:
    BasicStoneCalculator() = default;
    BasicStoneCalculator(const Table& table);

    BasicStoneCalculator(const BasicStoneCalculator&) = default;
    BasicStoneCalculator& operator=(const BasicStoneCalculator&) = default;
    BasicStoneCalculator(BasicStoneCalculator&&) = default;
    BasicStoneCalculator& operator=(BasicStoneCalculator&&) = default;

    float operator()(const Status& status, Point p) const;

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int /*version*/)
    {
        ar & distances_;
    }
};

using BasicHeurCalculator = TableHeurCalculator<BasicStoneCalculator>;

} // namespace sokoban

#endif /* BASICHEURCALCULATOR_H_ */
