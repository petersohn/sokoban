#ifndef SRC_STATUS_FLOODFILL_HPP
#define SRC_STATUS_FLOODFILL_HPP

#include "Status.hpp"

#include <ostream>

namespace sokoban {

struct MinMax {
    int minX, maxX, minY, maxY;
};

void floodFill(const Status& status, Point p0, Matrix<bool>& result);
void floodFill(const Status& status, Point p0, Matrix<bool>& result,
            Status::BorderType& border);
void floodFill(const Status& status, Point p0, Matrix<bool>& result,
            MinMax& minmax);
void floodFill(const Status& status, Point p0, Matrix<bool>& result,
            MinMax& minmax, Status::BorderType& border);

inline
bool operator==(const MinMax& lhs, const MinMax& rhs)
{
    return
        lhs.minX == rhs.minX &&
        lhs.maxX == rhs.maxX &&
        lhs.minY == rhs.minY &&
        lhs.maxY == rhs.maxY;
}

std::ostream& operator<<(std::ostream& os, const MinMax& minmax);

} // namespace sokoban

#endif /* SRC_STATUS_FLOODFILL_HPP */
