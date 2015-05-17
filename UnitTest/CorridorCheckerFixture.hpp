#ifndef UNITTEST_CORRIDORCHECKERFIXTURE_HPP
#define UNITTEST_CORRIDORCHECKERFIXTURE_HPP

#include "MockHeurCalculator.hpp"

#include <memory>

template <typename Checker>
struct CorridorCheckerFixture {
    std::shared_ptr<MockHeurCalculator> heurCalculator =
        std::make_shared<MockHeurCalculator>();
    Checker corridorCheckerUnderTest{
        typename Checker::StrategyFactoryType{heurCalculator}};
};


#endif /* UNITTEST_CORRIDORCHECKERFIXTURE_HPP */
