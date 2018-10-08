#include "Status/Status.hpp"
#include "Status/StatusCreator.hpp"

#include "Exception.hpp"
#include "FieldType.hpp"

#include <boost/test/unit_test.hpp>

using namespace sokoban;

BOOST_AUTO_TEST_SUITE(StatusCreatorTest)

BOOST_AUTO_TEST_CASE(simple_table)
{
    auto data = createStatus({
            "x.o",
            "..y"
        });
    BOOST_CHECK_EQUAL(data.first->width(), 3);
    BOOST_CHECK_EQUAL(data.first->height(), 2);
    BOOST_CHECK_EQUAL(data.first->wall(Point{0, 0}), false);
    BOOST_CHECK_EQUAL(data.first->wall(Point{1, 0}), false);
    BOOST_CHECK_EQUAL(data.first->wall(Point{2, 0}), false);
    BOOST_CHECK_EQUAL(data.first->wall(Point{0, 1}), false);
    BOOST_CHECK_EQUAL(data.first->wall(Point{1, 1}), false);
    BOOST_CHECK_EQUAL(data.first->wall(Point{2, 1}), false);

    BOOST_CHECK_EQUAL(&data.second.table(), data.first.get());
    BOOST_CHECK_EQUAL(data.second.state().size(), 1);
    BOOST_CHECK_EQUAL(data.second.value(Point{2, 0}), FieldType::stone);
    BOOST_CHECK_EQUAL(data.second.currentPos(), (Point{2, 1}));
}

BOOST_AUTO_TEST_CASE(walls)
{
    auto data = createStatus({
            "x.o",
            "*y*"
        });
    BOOST_CHECK_EQUAL(data.first->width(), 3);
    BOOST_CHECK_EQUAL(data.first->height(), 2);
    BOOST_CHECK_EQUAL(data.first->wall(Point{0, 0}), false);
    BOOST_CHECK_EQUAL(data.first->wall(Point{1, 0}), false);
    BOOST_CHECK_EQUAL(data.first->wall(Point{2, 0}), false);
    BOOST_CHECK_EQUAL(data.first->wall(Point{0, 1}), true);
    BOOST_CHECK_EQUAL(data.first->wall(Point{1, 1}), false);
    BOOST_CHECK_EQUAL(data.first->wall(Point{2, 1}), true);

    BOOST_CHECK_EQUAL(&data.second.table(), data.first.get());
    BOOST_CHECK_EQUAL(data.second.state().size(), 1);
    BOOST_CHECK_EQUAL(data.second.value(Point{2, 0}), FieldType::stone);
    BOOST_CHECK_EQUAL(data.second.currentPos(), (Point{1, 1}));
}

BOOST_AUTO_TEST_CASE(trim)
{
    auto data = createStatus({
            "x.o**",
            ".y.**",
            "*****",
        });
    BOOST_CHECK_EQUAL(data.first->width(), 3);
    BOOST_CHECK_EQUAL(data.first->height(), 2);
}

BOOST_AUTO_TEST_CASE(remove_unreachable_points)
{
    auto data = createStatus({
            "o.*xy*.",
            ".o*o**.",
        });
    BOOST_CHECK_EQUAL(data.first->width(), 5);
    BOOST_CHECK_EQUAL(data.first->height(), 2);
    BOOST_CHECK_EQUAL(data.first->wall(Point{0, 0}), true);
    BOOST_CHECK_EQUAL(data.first->wall(Point{1, 0}), true);
    BOOST_CHECK_EQUAL(data.first->wall(Point{2, 0}), true);
    BOOST_CHECK_EQUAL(data.first->wall(Point{3, 0}), false);
    BOOST_CHECK_EQUAL(data.first->wall(Point{4, 0}), false);
    BOOST_CHECK_EQUAL(data.first->wall(Point{0, 1}), true);
    BOOST_CHECK_EQUAL(data.first->wall(Point{1, 1}), true);
    BOOST_CHECK_EQUAL(data.first->wall(Point{2, 1}), true);
    BOOST_CHECK_EQUAL(data.first->wall(Point{3, 1}), false);
    BOOST_CHECK_EQUAL(data.first->wall(Point{4, 1}), true);
    BOOST_CHECK_EQUAL(data.second.state().size(), 1);
}

BOOST_AUTO_TEST_CASE(no_lines)
{
    BOOST_CHECK_THROW(createStatus({}), SokobanException);
}

BOOST_AUTO_TEST_CASE(no_columns)
{
    BOOST_CHECK_THROW(createStatus({"", "", ""}), SokobanException);
}

BOOST_AUTO_TEST_CASE(no_nonwalls)
{
    BOOST_CHECK_THROW(createStatus({
            "**",
            "**",
            "**"}), SokobanException);
}

BOOST_AUTO_TEST_CASE(multiple_starting_points)
{
    BOOST_CHECK_THROW(createStatus({
            "oy",
            "x.",
            "y."}), SokobanException);
}

BOOST_AUTO_TEST_CASE(multiple_destination)
{
    BOOST_CHECK_THROW(createStatus({
            "ox",
            "x.",
            "y."}), SokobanException);
}

BOOST_AUTO_TEST_CASE(split)
{
    BOOST_CHECK_THROW(createStatus({
            "x.o",
            "***",
            "yo."}), SokobanException);
}

BOOST_AUTO_TEST_CASE(missing_destination)
{
    BOOST_CHECK_THROW(createStatus({
            "..o",
            ".y."
        }), SokobanException);
}

BOOST_AUTO_TEST_CASE(missing_starting_position)
{
    BOOST_CHECK_THROW(createStatus({
            "x.o",
            "..."
        }), SokobanException);
}

BOOST_AUTO_TEST_CASE(missing_stones)
{
    BOOST_CHECK_THROW(createStatus({
            "x..",
            ".y."
        }), SokobanException);
}

BOOST_AUTO_TEST_SUITE_END()

