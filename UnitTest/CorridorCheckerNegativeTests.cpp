#include "Checker/CorridorChecker.hpp"
#include "Checker/ExtendedCorridorChecker.hpp"

#include "Status/StatusCreator.hpp"

#include "CheckerTest.hpp"
#include "CorridorCheckerFixture.hpp"

#include <boost/mpl/list.hpp>
#include <boost/test/unit_test.hpp>

using namespace sokoban;

BOOST_AUTO_TEST_SUITE(CorridorCheckerCommonTests)

BOOST_AUTO_TEST_SUITE(NegativeTests)

using checkerTypes = boost::mpl::list<CorridorChecker, ExtendedCorridorChecker>;

BOOST_AUTO_TEST_CASE_TEMPLATE(blocked_when_1x1_corridor_cannot_be_opened1, Checker, checkerTypes)
{
    CorridorCheckerFixture<Checker> fixture;
    auto data = createStatus({
            "y....",
            "..oo.",
            ".o.o.",
            ".oo..",
            "x...."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.corridorCheckerUnderTest, data.second, false);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(blocked_when_1x1_corridor_cannot_be_opened2, Checker, checkerTypes)
{
    CorridorCheckerFixture<Checker> fixture;
    auto data = createStatus({
            "y....",
            ".oo..",
            ".o.o.",
            "..oo.",
            "x...."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.corridorCheckerUnderTest, data.second, false);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(blocked_when_openings_are_not_movable, Checker, checkerTypes)
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
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{1, 3}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{3, 1}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{3, 3}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.corridorCheckerUnderTest, data.second, false);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(blocked_when_horizontal_corridor_cannot_be_opened1, Checker, checkerTypes)
{
    CorridorCheckerFixture<Checker> fixture;
    auto data = createStatus({
            ".......",
            ".y.....",
            ".ooooo.",
            ".o...o.",
            "..ooo..",
            ".x.....",
            "......."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.corridorCheckerUnderTest, data.second, false);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(blocked_when_horizontal_corridor_cannot_be_opened2, Checker, checkerTypes)
{
    CorridorCheckerFixture<Checker> fixture;
    auto data = createStatus({
            ".......",
            ".y.....",
            "..oooo.",
            ".o...o.",
            ".oooo..",
            ".x.....",
            "......."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.corridorCheckerUnderTest, data.second, false);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(blocked_when_openings_are_not_movable_horizontally, Checker, checkerTypes)
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
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{5, 2}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{5, 4}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.corridorCheckerUnderTest, data.second, false);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(blocked_when_vertical_corridor_cannot_be_opened1, Checker, checkerTypes)
{
    CorridorCheckerFixture<Checker> fixture;
    auto data = createStatus({
            ".......",
            ".yoo...",
            "..o.o..",
            "..o.o..",
            "..o.o..",
            ".xoo...",
            "......."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.corridorCheckerUnderTest, data.second, false);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(blocked_when_vertical_corridor_cannot_be_opened2, Checker, checkerTypes)
{
    CorridorCheckerFixture<Checker> fixture;
    auto data = createStatus({
            ".......",
            ".yoo...",
            "..o.o..",
            "..o.o..",
            "..o.o..",
            ".x.oo..",
            "......."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.corridorCheckerUnderTest, data.second, false);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(blocked_when_openings_are_not_movable_vertically, Checker, checkerTypes)
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
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{2, 5}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).with(mock::any, Point{4, 5}).returns(-1);
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.corridorCheckerUnderTest, data.second, false);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(blocked_when_corridor_cannot_be_opened_against_wall1, Checker, checkerTypes)
{
    CorridorCheckerFixture<Checker> fixture;
    auto data = createStatus({
            "y....",
            "..ooo",
            "..o..",
            "...oo",
            "x...."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.corridorCheckerUnderTest, data.second, false);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(blocked_when_corridor_cannot_be_opened_against_wall2, Checker, checkerTypes)
{
    CorridorCheckerFixture<Checker> fixture;
    auto data = createStatus({
            "y....",
            ".....",
            ".oo..",
            ".o.o.",
            "xo.o."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.corridorCheckerUnderTest, data.second, false);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(blocked_when_corridor_cannot_be_opened_against_wall3, Checker, checkerTypes)
{
    CorridorCheckerFixture<Checker> fixture;
    auto data = createStatus({
            "y....",
            "ooo..",
            "..o..",
            "oo...",
            "x...."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.corridorCheckerUnderTest, data.second, false);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(blocked_when_corridor_cannot_be_opened_against_wall4, Checker, checkerTypes)
{
    CorridorCheckerFixture<Checker> fixture;
    auto data = createStatus({
            "yo.o.",
            ".o.o.",
            "..oo.",
            ".....",
            "x...."
        });
    MOCK_EXPECT(fixture.heurCalculator->calculateStone).returns(1);
    CHECK_CHECKER_RESULT(fixture.corridorCheckerUnderTest, data.second, false);
}


BOOST_AUTO_TEST_SUITE_END() // NegativeTests

BOOST_AUTO_TEST_SUITE_END() // CorridorCheckerCommonTests


