#include "Checker/ExtendedCorridorChecker.hpp"

#include "Status/Status.hpp"
#include "Status/StatusCreator.hpp"

#include "CheckerTest.hpp"
#include "CorridorCheckerFixture.hpp"
#include "MockHeurCalculator.hpp"

#include <boost/test/unit_test.hpp>

using namespace sokoban;

BOOST_FIXTURE_TEST_SUITE(ExtendedCorridorCheckerTest,
        CorridorCheckerFixture<ExtendedCorridorChecker>)

BOOST_AUTO_TEST_CASE(blocked_when_opening_source_points_are_not_reachable_horizontally)
{
    auto data = createStatus({
            "y....",
            ".ooo.",
            "o...o",
            ".ooo.",
            "x...."
        });
    MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{0, 3}).returns(-1);
    MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{4, 3}).returns(-1);
    MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(corridorCheckerUnderTest, data.second, false);
}

BOOST_AUTO_TEST_CASE(blocked_when_opening_source_points_are_not_reachable_vertically)
{
    auto data = createStatus({
            "y.o..",
            ".o.o.",
            ".o.o.",
            ".o.o.",
            "x.o.."
        });
    MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{3, 0}).returns(-1);
    MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{3, 4}).returns(-1);
    MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(corridorCheckerUnderTest, data.second, false);
}



BOOST_AUTO_TEST_SUITE_END()

