#ifndef UNITTEST_CREATEBOOLARRAY_HPP
#define UNITTEST_CREATEBOOLARRAY_HPP

#include "Array.hpp"

template <typename Table>
Array<bool> createBoolArray(const Table& table,
		const std::vector<Point>& trueList)
{
	Array<bool> result{table.width(), table.height(), false};

	for (Point p: trueList) {
		result[p] = true;
	}

	return result;
}


#endif /* UNITTEST_CREATEBOOLARRAY_HPP */
