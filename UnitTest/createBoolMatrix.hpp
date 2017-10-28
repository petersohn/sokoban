#ifndef UNITTEST_CREATEBOOLMATRIX_HPP
#define UNITTEST_CREATEBOOLMATRIX_HPP

#include "Matrix.hpp"

namespace sokoban {

template <typename Table>
Matrix<bool> createBoolMatrix(const Table& table,
        const std::vector<Point>& trueList)
{
    Matrix<bool> result{table.width(), table.height(), false};

    for (Point p: trueList) {
        result[p] = true;
    }

    return result;
}

} // namespace sokoban

#endif // UNITTEST_CREATEBOOLMATRIX_HPP
