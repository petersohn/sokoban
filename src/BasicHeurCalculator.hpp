#ifndef BASICHEURCALCULATOR_H_
#define BASICHEURCALCULATOR_H_

#include "TableHeurCalculator.hpp"
#include "Array.hpp"

class BasicStoneCalculator {
    Array<float> distances_;
    bool checkDistance(const Table& table, Point  p, Point  d, int dist);
public:
    BasicStoneCalculator(const Table& table);

    BasicStoneCalculator(const BasicStoneCalculator&) = default;
    BasicStoneCalculator& operator=(const BasicStoneCalculator&) = default;
    BasicStoneCalculator(BasicStoneCalculator&&) = default;
    BasicStoneCalculator& operator=(BasicStoneCalculator&&) = default;

    float operator()(const Status& status, Point p) const;
};

using BasicHeurCalculator = TableHeurCalculator<BasicStoneCalculator>;

#endif /* BASICHEURCALCULATOR_H_ */
