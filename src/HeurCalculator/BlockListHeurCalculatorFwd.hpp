#ifndef HEURCALCULATOR_BLOCKLISTHEURCALCULATORFWD_HPP
#define HEURCALCULATOR_BLOCKLISTHEURCALCULATORFWD_HPP

#include "HeurCalculator/SubStatusHeurCalculatorFwd.hpp"

namespace sokoban {

class BlockListHeurListFactory;

using BlockListHeurCalculator =
        SubStatusHeurCalculator<BlockListHeurListFactory>;

} // namespace sokoban

#endif // HEURCALCULATOR_BLOCKLISTHEURCALCULATORFWD_HPP
