#ifndef HEURCALCULATOR_DECISIONHEURCALCULATORFWD_HPP
#define HEURCALCULATOR_DECISIONHEURCALCULATORFWD_HPP

#include "HeurCalculator/SubStatusHeurCalculatorFwd.hpp"

class DecisionTreeHeurListFactory;

using DecisionTreeHeurCalculator = 
        SubStatusHeurCalculator<DecisionTreeHeurListFactory>;

#endif // HEURCALCULATOR_DECISIONHEURCALCULATORFWD_HPP
