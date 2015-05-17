#include "Checker/ExtendedMovableChecker.hpp"
#include "Checker/MovableChecker.hpp"

#include "Status/StatusCreator.hpp"

#include "CheckerTest.hpp"
#include "MovableCheckerFixture.hpp"

#include <boost/mpl/list.hpp>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(MovableCheckerCommonTests)

BOOST_AUTO_TEST_SUITE(PositiveTests)

using checkerTypes = boost::mpl::list<MovableChecker, ExtendedMovableChecker>;

BOOST_AUTO_TEST_CASE_TEMPLATE(not_blocked_when_there_is_a_single_stone_in_the_middle, Checker, checkerTypes)
{
    MovableCheckerFixture<Checker> fixture;
    auto data = createStatus(5, 5, {
            "y....",
            ".....",
            "..o..",
            ".....",
            "x...."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.movableCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(not_blocked_when_there_are_multiple_stones_in_the_middle_horizontally, Checker, checkerTypes)
{
    MovableCheckerFixture<Checker> fixture;
    auto data = createStatus(5, 5, {
            "y....",
            ".....",
            ".ooo.",
            ".....",
            "x...."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.movableCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(not_blocked_when_there_are_multiple_stones_in_the_middle_vertically, Checker, checkerTypes)
{
    MovableCheckerFixture<Checker> fixture;
    auto data = createStatus(5, 5, {
            "y....",
            "..o..",
            "..o..",
            "..o..",
            "x...."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.movableCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(not_blocked_when_there_are_multiple_stones_in_the_middle_in_different_shapes, Checker, checkerTypes)
{
    MovableCheckerFixture<Checker> fixture;
    auto data = createStatus(5, 5, {
            "y....",
            ".oo..",
            "..o..",
            "..oo.",
            "x...."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.movableCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(not_blocked_when_there_is_a_stone_surrounded_by_movable_stones, Checker, checkerTypes)
{
    MovableCheckerFixture<Checker> fixture;
    auto data = createStatus(5, 5, {
            "y....",
            "..o..",
            ".ooo.",
            "..o..",
            "x...."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.movableCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(not_blocked_when_there_is_a_stone_surrounded_by_walls_vertically, Checker, checkerTypes)
{
    MovableCheckerFixture<Checker> fixture;
    auto data = createStatus(5, 5, {
            "y....",
            ".*.*.",
            ".*o*.",
            ".*.*.",
            "x...."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.movableCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(not_blocked_when_there_is_a_stone_surrounded_by_walls_horizontally, Checker, checkerTypes)
{
    MovableCheckerFixture<Checker> fixture;
    auto data = createStatus(5, 5, {
            "y....",
            ".***.",
            "..o..",
            ".***.",
            "x...."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.movableCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(not_blocked_when_there_is_a_stone_surrounded_by_walls_vertically_and_movable_in_one_direction, Checker, checkerTypes)
{
    MovableCheckerFixture<Checker> fixture;
    auto data = createStatus(5, 5, {
            "y....",
            ".*.*.",
            ".*o*.",
            ".*.*.",
            "x...."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{2, 1}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.movableCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(not_blocked_when_there_is_a_stone_surrounded_by_walls_horizontally_and_movable_in_one_direction, Checker, checkerTypes)
{
    MovableCheckerFixture<Checker> fixture;
    auto data = createStatus(5, 5, {
            "y....",
            ".***.",
            "..o..",
            ".***.",
            "x...."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{1, 2}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.movableCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(not_blocked_when_there_is_a_stone_surrounded_by_walls_vertically_and_movable_in_another_direction, Checker, checkerTypes)
{
    MovableCheckerFixture<Checker> fixture;
    auto data = createStatus(5, 5, {
            "y....",
            ".*.*.",
            ".*o*.",
            ".*.*.",
            "x...."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{2, 3}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.movableCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(not_blocked_when_there_is_a_stone_surrounded_by_walls_horizontally_and_movable_in_another_direction, Checker, checkerTypes)
{
    MovableCheckerFixture<Checker> fixture;
    auto data = createStatus(5, 5, {
            "y....",
            ".***.",
            "..o..",
            ".***.",
            "x...."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{3, 2}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.movableCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(not_blocked_when_there_are_multiple_stones_movable_in_one_direction_horizonally, Checker, checkerTypes)
{
    MovableCheckerFixture<Checker> fixture;
    auto data = createStatus(5, 5, {
            "y....",
            ".....",
            ".ooo.",
            ".....",
            "x...."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{1, 3}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{2, 3}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{3, 3}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.movableCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(not_blocked_when_there_are_multiple_stones_movable_in_one_direction_vertically, Checker, checkerTypes)
{
    MovableCheckerFixture<Checker> fixture;
    auto data = createStatus(5, 5, {
            "y....",
            "..o..",
            "..o..",
            "..o..",
            "x...."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{3, 1}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{3, 2}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{3, 3}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.movableCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(not_blocked_when_there_are_multiple_stones_movable_in_another_direction_horizonally, Checker, checkerTypes)
{
    MovableCheckerFixture<Checker> fixture;
    auto data = createStatus(5, 5, {
            "y....",
            ".....",
            ".ooo.",
            ".....",
            "x...."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{1, 1}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{2, 1}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{3, 1}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.movableCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(not_blocked_when_there_are_multiple_stones_movable_in_another_direction_vertically, Checker, checkerTypes)
{
    MovableCheckerFixture<Checker> fixture;
    auto data = createStatus(5, 5, {
            "y....",
            "..o..",
            "..o..",
            "..o..",
            "x...."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{1, 1}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{1, 2}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{1, 3}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.movableCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(not_blocked_when_stone_is_reachable_indirectly_vertically, Checker, checkerTypes)
{
    MovableCheckerFixture<Checker> fixture;
    auto data = createStatus(5, 5, {
            "y.*..",
            "..o..",
            "..*..",
            "..oo.",
            "x.*.."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{4, 3}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.movableCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(not_blocked_when_stone_is_reachable_indirectly_horizontally, Checker, checkerTypes)
{
    MovableCheckerFixture<Checker> fixture;
    auto data = createStatus(5, 5, {
            "y....",
            ".....",
            "*o*o*",
            "...o.",
            "x...."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{3, 4}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.movableCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(not_blocked_when_stone_is_reachable_from_one_direction_horizontally1, Checker, checkerTypes)
{
    MovableCheckerFixture<Checker> fixture;
    auto data = createStatus(5, 5, {
            "..*.y",
            "..*..",
            ".o*..",
            "xo...",
            ".*..."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.movableCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(not_blocked_when_stone_is_reachable_from_one_direction_horizontally2, Checker, checkerTypes)
{
    MovableCheckerFixture<Checker> fixture;
    auto data = createStatus(5, 5, {
            "y.*..",
            "..*..",
            "..*o.",
            "...ox",
            "..*.."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.movableCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(not_blocked_when_stone_is_reachable_from_one_direction_vertically1, Checker, checkerTypes)
{
    MovableCheckerFixture<Checker> fixture;
    auto data = createStatus(5, 5, {
            "y....",
            ".....",
            "..***",
            "*oo..",
            ".x..."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.movableCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(not_blocked_when_stone_is_reachable_from_one_direction_vertically2, Checker, checkerTypes)
{
    MovableCheckerFixture<Checker> fixture;
    auto data = createStatus(5, 5, {
            ".x...",
            "*oo..",
            "..***",
            ".....",
            "y...."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.movableCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()


