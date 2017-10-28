#ifndef HEURCALCULATOR_DECISIONHEURCALCULATORFWD_HPP
#define HEURCALCULATOR_DECISIONHEURCALCULATORFWD_HPP

#include "HeurCalculator/SubStatusHeurCalculatorFwd.hpp"

namespace sokoban {

class DecisionTreeHeurListFactory;

using DecisionTreeHeurCalculator =
        SubStatusHeurCalculator<DecisionTreeHeurListFactory>;

} // namespace sokoban

#endif // HEURCALCULATOR_DECISIONHEURCALCULATORFWD_HPP
