#ifndef SRC_CHOKEPOINTFINDER_HPP
#define SRC_CHOKEPOINTFINDER_HPP

#include "Matrix.hpp"
#include "Options.hpp"
#include "Checker/ComplexCheckerFwd.hpp"
#include "Status/Point.hpp"

#include <vector>

namespace sokoban {

class Table;
class HeurCalculator;

Matrix<bool> findChokePoints(const Table& table, Options options,
        std::shared_ptr<const HeurCalculator> calculator,
        ComplexChecker checker, bool print);

} // namespace sokoban

#endif /* SRC_CHOKEPOINTFINDER_HPP */
