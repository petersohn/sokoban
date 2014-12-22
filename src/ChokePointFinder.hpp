#ifndef SRC_CHOKEPOINTFINDER_HPP
#define SRC_CHOKEPOINTFINDER_HPP

#include "Options.hpp"
#include <vector>

class Table;
class Point;
class HeurCalculator;
class ComplexChecker;

std::vector<Point> findChokePoints(const Table& table, Options options,
	    std::shared_ptr<const HeurCalculator> calculator,
	    ComplexChecker checker, bool print);

#endif /* SRC_CHOKEPOINTFINDER_HPP */
