#ifndef UNITTEST_CORRIDORCHECKERFIXTURE_HPP
#define UNITTEST_CORRIDORCHECKERFIXTURE_HPP

#include "MockHeurCalculator.hpp"

#include <memory>

namespace sokoban {

template <typename Checker>
struct CorridorCheckerFixture {
    std::shared_ptr<MockHeurCalculator> heurCalculator =
        std::make_shared<MockHeurCalculator>();
    Checker corridorCheckerUnderTest{
        typename Checker::StrategyFactoryType{heurCalculator}};
};

} // namespace sokoban

#endif /* UNITTEST_CORRIDORCHECKERFIXTURE_HPP */
