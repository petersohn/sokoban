#include "Checker/CorridorChecker.hpp"
#include "Checker/ExtendedCorridorChecker.hpp"

#include "Status/StatusCreator.hpp"

#include "CheckerTest.hpp"
#include "CorridorCheckerFixture.hpp"

#include <boost/mpl/list.hpp>
#include <boost/test/unit_test.hpp>

using namespace sokoban;

BOOST_AUTO_TEST_SUITE(CorridorCheckerCommonTests)

BOOST_AUTO_TEST_SUITE(PositiveTests)

using checkerTypes = boost::mpl::list<CorridorChecker, ExtendedCorridorChecker>;

BOOST_AUTO_TEST_CASE_TEMPLATE(not_blocked_when_there_is_a_single_stone_in_the_middle, Checker, checkerTypes)
{
    CorridorCheckerFixture<Checker> fixture;
    auto data = createStatus({
            "y....",
            ".....",
            "..o..",
            ".....",
            "x...."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(not_blocked_when_1x1_corridor_can_be_opened_from_all_directions, Checker, checkerTypes)
{
    CorridorCheckerFixture<Checker> fixture;
    auto data = createStatus({
            "y....",
            "..o..",
            ".o.o.",
            "..o..",
            "x...."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(not_blocked_when_1x1_corridor_can_be_opened_from_two_directions1, Checker, checkerTypes)
{
    CorridorCheckerFixture<Checker> fixture;
    auto data = createStatus({
            "y....",
            "..oo.",
            ".o.o.",
            "..o..",
            "x...."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(not_blocked_when_1x1_corridor_can_be_opened_from_two_directions2, Checker, checkerTypes)
{
    CorridorCheckerFixture<Checker> fixture;
    auto data = createStatus({
            "y....",
            "..o..",
            ".o.o.",
            "..oo.",
            "x...."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(not_blocked_when_1x1_corridor_can_be_opened_from_two_directions3, Checker, checkerTypes)
{
    CorridorCheckerFixture<Checker> fixture;
    auto data = createStatus({
            "y....",
            "..o..",
            ".o.o.",
            ".oo..",
            "x...."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(not_blocked_when_1x1_corridor_can_be_opened_from_two_directions4, Checker, checkerTypes)
{
    CorridorCheckerFixture<Checker> fixture;
    auto data = createStatus({
            "y....",
            ".oo..",
            ".o.o.",
            "..o..",
            "x...."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(not_blocked_when_1x1_corridor_can_be_opened_from_one_direction1, Checker, checkerTypes)
{
    CorridorCheckerFixture<Checker> fixture;
    auto data = createStatus({
            "y....",
            ".ooo.",
            ".o.o.",
            "..o..",
            "x...."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(not_blocked_when_1x1_corridor_can_be_opened_from_one_direction2, Checker, checkerTypes)
{
    CorridorCheckerFixture<Checker> fixture;
    auto data = createStatus({
            "y....",
            "..oo.",
            ".o.o.",
            "..oo.",
            "x...."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(not_blocked_when_1x1_corridor_can_be_opened_from_one_direction3, Checker, checkerTypes)
{
    CorridorCheckerFixture<Checker> fixture;
    auto data = createStatus({
            "y....",
            "..o..",
            ".o.o.",
            ".ooo.",
            "x...."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(not_blocked_when_1x1_corridor_can_be_opened_from_one_direction4, Checker, checkerTypes)
{
    CorridorCheckerFixture<Checker> fixture;
    auto data = createStatus({
            "y....",
            ".oo..",
            ".o.o.",
            ".oo..",
            "x...."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(not_blocked_when_openings_are_movable_from_one_direction, Checker, checkerTypes)
{
    CorridorCheckerFixture<Checker> fixture;
    auto data = createStatus({
            "y....",
            "..o..",
            ".o.o.",
            "..o..",
            "x...."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{1, 1}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{3, 3}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(not_blocked_when_openings_are_movable_from_another_direction, Checker, checkerTypes)
{
    CorridorCheckerFixture<Checker> fixture;
    auto data = createStatus({
            "y....",
            "..o..",
            ".o.o.",
            "..o..",
            "x...."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{1, 3}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{3, 1}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(not_blocked_when_horizontal_corridor_can_be_opened_from_all_directions, Checker, checkerTypes)
{
    CorridorCheckerFixture<Checker> fixture;
    auto data = createStatus({
            ".......",
            ".y.....",
            "..ooo..",
            ".o...o.",
            "..ooo..",
            ".x.....",
            "......."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(not_blocked_when_horizontal_corridor_can_be_opened_from_one_direction, Checker, checkerTypes)
{
    CorridorCheckerFixture<Checker> fixture;
    auto data = createStatus({
            ".......",
            ".y.....",
            "..ooo..",
            ".o...o.",
            "..oooo.",
            ".x.....",
            "......."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(not_blocked_when_horizontal_corridor_can_be_opened_from_another_direction, Checker, checkerTypes)
{
    CorridorCheckerFixture<Checker> fixture;
    auto data = createStatus({
            ".......",
            ".y.....",
            ".oooo..",
            ".o...o.",
            "..ooo..",
            ".x.....",
            "......."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(not_blocked_when_openings_are_movable_from_one_direction_horizontally, Checker, checkerTypes)
{
    CorridorCheckerFixture<Checker> fixture;
    auto data = createStatus({
            ".......",
            ".y.....",
            "..ooo..",
            ".o...o.",
            "..ooo..",
            ".x.....",
            "......."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{1, 2}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{1, 4}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{5, 4}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(not_blocked_when_openings_are_movable_from_other_direction_horizontally, Checker, checkerTypes)
{
    CorridorCheckerFixture<Checker> fixture;
    auto data = createStatus({
            ".......",
            ".y.....",
            "..ooo..",
            ".o...o.",
            "..ooo..",
            ".x.....",
            "......."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{1, 2}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{5, 2}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{5, 4}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(not_blocked_when_vertical_corridor_can_be_opened_from_all_directions, Checker, checkerTypes)
{
    CorridorCheckerFixture<Checker> fixture;
    auto data = createStatus({
            ".......",
            ".y.o...",
            "..o.o..",
            "..o.o..",
            "..o.o..",
            ".x.o...",
            "......."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(not_blocked_when_vertical_corridor_can_be_opened_from_one_direction, Checker, checkerTypes)
{
    CorridorCheckerFixture<Checker> fixture;
    auto data = createStatus({
            ".......",
            ".y.o...",
            "..o.o..",
            "..o.o..",
            "..o.o..",
            ".x.oo..",
            "......."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(not_blocked_when_vertical_corridor_can_be_opened_from_another_direction, Checker, checkerTypes)
{
    CorridorCheckerFixture<Checker> fixture;
    auto data = createStatus({
            ".......",
            ".yoo...",
            "..o.o..",
            "..o.o..",
            "..o.o..",
            ".x.o...",
            "......."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(not_blocked_when_openings_are_movable_from_one_direction_vertically, Checker, checkerTypes)
{
    CorridorCheckerFixture<Checker> fixture;
    auto data = createStatus({
            ".......",
            ".y.o...",
            "..o.o..",
            "..o.o..",
            "..o.o..",
            ".x.o...",
            "......."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{2, 1}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{4, 1}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{4, 5}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(not_blocked_when_openings_are_movable_from_other_direction_vertically, Checker, checkerTypes)
{
    CorridorCheckerFixture<Checker> fixture;
    auto data = createStatus({
            ".......",
            ".y.o...",
            "..o.o..",
            "..o.o..",
            "..o.o..",
            ".x.o...",
            "......."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{2, 1}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{2, 5}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{4, 5}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(not_blocked_when_corridor_can_be_opened_against_wall1, Checker, checkerTypes)
{
    CorridorCheckerFixture<Checker> fixture;
    auto data = createStatus({
            "y....",
            "...oo",
            "..o..",
            "...oo",
            "x...."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(not_blocked_when_corridor_can_be_opened_against_wall2, Checker, checkerTypes)
{
    CorridorCheckerFixture<Checker> fixture;
    auto data = createStatus({
            "y....",
            ".....",
            "..o..",
            ".o.o.",
            "xo.o."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(not_blocked_when_corridor_can_be_opened_against_wall3, Checker, checkerTypes)
{
    CorridorCheckerFixture<Checker> fixture;
    auto data = createStatus({
            "y....",
            "oo...",
            "..o..",
            "oo...",
            "x...."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(not_blocked_when_corridor_can_be_opened_against_wall4, Checker, checkerTypes)
{
    CorridorCheckerFixture<Checker> fixture;
    auto data = createStatus({
            "yo.o.",
            ".o.o.",
            "..o..",
            ".....",
            "x...."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(not_blocked_when_opening_target_points_are_not_reachable_horizontally, Checker, checkerTypes)
{
    CorridorCheckerFixture<Checker> fixture;
    auto data = createStatus({
            "y....",
            ".ooo.",
            "o...o",
            ".ooo.",
            "x...."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{0, 1}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{4, 1}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(not_blocked_when_opening_target_points_are_not_reachable_vertically, Checker, checkerTypes)
{
    CorridorCheckerFixture<Checker> fixture;
    auto data = createStatus({
            "y.o..",
            ".o.o.",
            ".o.o.",
            ".o.o.",
            "x.o.."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{1, 0}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{1, 4}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(not_blocked_when_opening_source_points_are_reachable_indirectly_horizontally, Checker, checkerTypes)
{
    CorridorCheckerFixture<Checker> fixture;
    auto data = createStatus({
            "y....",
            "o....",
            ".o*o*",
            "o....",
            "x...."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{1, 3}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(not_blocked_when_opening_source_points_are_reachable_indirectly_vertically, Checker, checkerTypes)
{
    CorridorCheckerFixture<Checker> fixture;
    auto data = createStatus({
            "y.*..",
            "..o..",
            "..*..",
            "..o..",
            ".o.ox"
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{3, 3}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.corridorCheckerUnderTest, data.second, true);
}


BOOST_AUTO_TEST_SUITE_END() // PositiveTests

BOOST_AUTO_TEST_SUITE_END() // CorridorCheckerCommonTests

