#include "Status/IsStatusPossible.hpp"
#include "Status/Status.hpp"
#include "Status/StatusCreator.hpp"

#include <boost/test/unit_test.hpp>

using namespace sokoban;

BOOST_AUTO_TEST_SUITE(IsStatusPossibleTest)

BOOST_AUTO_TEST_CASE(possible_when_point_is_near_destination)
{
    auto data = createStatus({
            "y.o.",
            "x.o."
        });
    data.second.currentPos(Point{1, 0});
    BOOST_CHECK(isStatusPossible(data.second, 1));
}

BOOST_AUTO_TEST_CASE(possible_when_point_is_on_destination)
{
    auto data = createStatus({
            "..oy",
            "x..o"
        });
    data.second.currentPos(Point{3, 0});
    BOOST_CHECK(isStatusPossible(data.second, 1));
}

BOOST_AUTO_TEST_CASE(possible_when_way_is_open)
{
    auto data = createStatus({
            "y.o..",
            ".ox.."
        });
    data.second.currentPos(Point{3, 0});
    BOOST_CHECK(isStatusPossible(data.second, 2));
}

BOOST_AUTO_TEST_CASE(impossible_when_point_is_isolated_not_on_destination)
{
    auto data = createStatus({
            "y.o.",
            "x..o"
        });
    data.second.currentPos(Point{3, 0});
    BOOST_CHECK(!isStatusPossible(data.second, 1));
}

BOOST_AUTO_TEST_CASE(possible_when_maxDepth_is_too_small)
{
    auto data = createStatus({
            "yo..",
            "x.oo"
        });
    data.second.currentPos(Point{3, 0});
    BOOST_CHECK(isStatusPossible(data.second, 1));
}

BOOST_AUTO_TEST_CASE(impossible_when_maxDepth_is_big_enough)
{
    auto data = createStatus({
            "yo..",
            "x.oo"
        });
    data.second.currentPos(Point{3, 0});
    BOOST_CHECK(!isStatusPossible(data.second, 2));
}

BOOST_AUTO_TEST_CASE(impossible_when_destination_is_in_the_way)
{
    auto data = createStatus({
            "yox.",
            "..oo"
        });
    data.second.currentPos(Point{3, 0});
    BOOST_CHECK(!isStatusPossible(data.second, 1));
}

BOOST_AUTO_TEST_CASE(complex_impossible1)
{
    auto data = createStatus({
            "....x",
            "..o..",
            ".o.o.",
            "..o..",
            "y...."
        });
    data.second.currentPos(Point{2, 2});
    BOOST_CHECK(!isStatusPossible(data.second, 1));
}

BOOST_AUTO_TEST_CASE(complex_impossible2)
{
    auto data = createStatus({
            "**..x",
            "..o..",
            "..o..",
            "**...",
            "y...."
        });
    data.second.currentPos(Point{0, 2});
    BOOST_CHECK(!isStatusPossible(data.second, 3));
}

BOOST_AUTO_TEST_CASE(complex_impossible3)
{
    auto data = createStatus({
            "x...*",
            "..oo.",
            "y*..."
        });
    data.second.currentPos(Point{4, 1});
    BOOST_CHECK(!isStatusPossible(data.second, 1));
}

BOOST_AUTO_TEST_SUITE_END() // IsSubStatus

