#include "Checker/ExtendedMovableChecker.hpp"

#include "Status/StatusCreator.hpp"

#include "CheckerTest.hpp"
#include "MockHeurCalculator.hpp"
#include "MovableCheckerFixture.hpp"

#include <boost/test/unit_test.hpp>

using namespace sokoban;

BOOST_FIXTURE_TEST_SUITE(ExtendedMovableCheckerTest,
        MovableCheckerFixture<ExtendedMovableChecker>)

BOOST_AUTO_TEST_CASE(blocked_when_source_point_not_reachable_vertically)
{
    auto data = createStatus(5, 5, {
            "y....",
            ".....",
            ".....",
            "..oo*",
            "x.*.."
        });
    MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{3, 4}).returns(-1);
    MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(movableCheckerUnderTest, data.second, false);
}

BOOST_AUTO_TEST_CASE(blocked_when_source_point_not_reachable_horizontally)
{
    auto data = createStatus(5, 5, {
            "y....",
            ".....",
            "...o*",
            "...o.",
            "x..*."
        });
    MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{4, 3}).returns(-1);
    MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(movableCheckerUnderTest, data.second, false);
}

BOOST_AUTO_TEST_SUITE_END()
