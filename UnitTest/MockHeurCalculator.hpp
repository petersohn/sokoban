#ifndef UNITTEST_MOCKHEURCALCULATOR_HPP
#define UNITTEST_MOCKHEURCALCULATOR_HPP

#include "Status/Status.hpp"
#include "HeurCalculator/HeurCalculator.hpp"

#include <turtle/mock.hpp>

MOCK_BASE_CLASS(MockHeurCalculator, sokoban::HeurCalculator) {
    MOCK_CONST_METHOD(calculateStatus, 3, float(
            const sokoban::Status&, const sokoban::MoveDescriptor*,
            const std::shared_ptr<sokoban::Node>&))
    MOCK_CONST_METHOD(calculateStone, 2, float(const sokoban::Status&,
            sokoban::Point))
};

#endif /* UNITTEST_MOCKHEURCALCULATOR_HPP */
