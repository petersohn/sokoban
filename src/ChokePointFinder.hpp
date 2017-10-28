#ifndef SRC_CHOKEPOINTFINDER_HPP
#define SRC_CHOKEPOINTFINDER_HPP

#include "Options.hpp"
#include "Checker/ComplexCheckerFwd.hpp"
#include <vector>

namespace sokoban {

class Table;
class Point;
class HeurCalculator;

template <typename T> class Array;

Array<bool> findChokePoints(const Table& table, Options options,
        std::shared_ptr<const HeurCalculator> calculator,
        ComplexChecker checker, bool print);

} // namespace sokoban

#endif /* SRC_CHOKEPOINTFINDER_HPP */
