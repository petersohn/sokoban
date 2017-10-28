#ifndef SRC_MOVABLECHECKERFIXTURE_HPP
#define SRC_MOVABLECHECKERFIXTURE_HPP

#include "MockHeurCalculator.hpp"

#include <memory>

namespace sokoban {

template <typename Checker>
struct MovableCheckerFixture {
    std::shared_ptr<MockHeurCalculator> heurCalculator =
        std::make_shared<MockHeurCalculator>();
    Checker movableCheckerUnderTest{heurCalculator};
};

} // namespace sokoban

#endif /* SRC_MOVABLECHECKERFIXTURE_HPP */
