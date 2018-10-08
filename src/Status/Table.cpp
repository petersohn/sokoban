#include "Dumper/DumperFunctions.hpp"

#include "Matrix.hpp"
#include "Status/Table.hpp"
#include "Status/Status.hpp"

namespace sokoban {

std::ostream& operator<<(std::ostream& os, const Table& table) {
    return os << Status{table};
}

void Table::trim() {
    std::size_t width = 0;
    std::size_t height = 0;
    for (Point p : matrixRange(walls_)) {
        if (!walls_[p]) {
            width = std::max(width, static_cast<std::size_t>(p.x + 1));
            height = std::max(height, static_cast<std::size_t>(p.y + 1));
        }
    }

    if (width != walls_.width() || height != walls_.height()) {
        Matrix<bool> newTable{width, height};
        for (Point p : matrixRange(newTable)) {
            newTable[p] = walls_[p];
        }
        walls_ = std::move(newTable);
    }
}

} // namespace sokoban
