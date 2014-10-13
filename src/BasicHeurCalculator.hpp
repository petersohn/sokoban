#ifndef BASICHEURCALCULATOR_H_
#define BASICHEURCALCULATOR_H_

#include "TableHeurCalculator.hpp"
#include "Array.hpp"

class BasicStoneCalculator {
	Array<int> distances_;
	bool checkDistance(const Table& table, Point  p, Point  d, int dist);
public:
	BasicStoneCalculator(const Table& table);

	BasicStoneCalculator(const BasicStoneCalculator&) = default;
	BasicStoneCalculator& operator=(const BasicStoneCalculator&) = default;
	BasicStoneCalculator(BasicStoneCalculator&&) = default;
	BasicStoneCalculator& operator=(BasicStoneCalculator&&) = default;

	int operator()(const Status &status, Point p);
};

using BasicHeurCalculator = TableHeurCalculator<BasicStoneCalculator>;

#endif /* BASICHEURCALCULATOR_H_ */
