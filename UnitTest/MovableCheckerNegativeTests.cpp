#include "Checker/MovableChecker.hpp"
#include "Checker/ExtendedMovableChecker.hpp"
#include "Status/StatusCreator.hpp"
#include "CheckerTest.hpp"
#include "MovableCheckerFixture.hpp"
#include <boost/test/unit_test.hpp>
#include <boost/mpl/list.hpp>

BOOST_AUTO_TEST_SUITE(MovableCheckerCommonTests)

BOOST_AUTO_TEST_SUITE(NegativeTests)

using checkerTypes = boost::mpl::list<MovableChecker, ExtendedMovableChecker>;

BOOST_AUTO_TEST_CASE_TEMPLATE(blocked_when_there_is_a_stone_surrounded_by_walls_vertically_and_movable_in_neither_direction, Checker, checkerTypes)
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
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{2, 3}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.movableCheckerUnderTest, data.second, false);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(blocked_when_there_is_a_stone_surrounded_by_walls_horizontally_and_movable_in_neither_direction, Checker, checkerTypes)
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
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{3, 2}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.movableCheckerUnderTest, data.second, false);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(blocked_when_a_stone_is_not_movable_vertically_and_blocked_by_stone_horizontally, Checker, checkerTypes)
{
    MovableCheckerFixture<Checker> fixture;
    auto data = createStatus(5, 5, {
            "y....",
            ".....",
            ".oo..",
            ".....",
            "x...."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{1, 1}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{1, 3}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{2, 1}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{2, 3}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.movableCheckerUnderTest, data.second, false);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(blocked_when_a_stone_is_not_movable_horizontally_and_blocked_by_stone_vertically, Checker, checkerTypes)
{
    MovableCheckerFixture<Checker> fixture;
    auto data = createStatus(5, 5, {
            "y....",
            "..o..",
            "..o..",
            ".....",
            "x...."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{1, 1}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{3, 1}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{1, 2}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{3, 2}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.movableCheckerUnderTest, data.second, false);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(blocked_when_there_are_multiple_stones_near_the_wall_vertically, Checker, checkerTypes)
{
    MovableCheckerFixture<Checker> fixture;
    auto data = createStatus(5, 5, {
            "y....",
            "..o*.",
            "..o*.",
            "...*.",
            "x...."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.movableCheckerUnderTest, data.second, false);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(blocked_when_there_are_multiple_stones_near_the_wall_horizontally, Checker, checkerTypes)
{
    MovableCheckerFixture<Checker> fixture;
    auto data = createStatus(5, 5, {
            "y....",
            ".***.",
            ".oo..",
            ".....",
            "x...."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.movableCheckerUnderTest, data.second, false);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(blocked_when_the_stones_are_blocking_each_other, Checker, checkerTypes)
{
    MovableCheckerFixture<Checker> fixture;
    auto data = createStatus(5, 5, {
            "y....",
            ".....",
            "..oo.",
            "..oo.",
            "x...."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.movableCheckerUnderTest, data.second, false);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()


