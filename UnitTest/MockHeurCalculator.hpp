#ifndef UNITTEST_MOCKHEURCALCULATOR_HPP
#define UNITTEST_MOCKHEURCALCULATOR_HPP

#include "HeurCalculator.hpp"
#include <turtle/mock.hpp>

MOCK_BASE_CLASS(MockHeurCalculator, HeurCalculator) {
	MOCK_CONST_METHOD(calculateStatus, 3, int(
			const Status&, const MoveDescriptor*, const std::shared_ptr<Node>&))
	MOCK_CONST_METHOD(calculateStone, 2, int(const Status&, Point))
};



#endif /* UNITTEST_MOCKHEURCALCULATOR_HPP */