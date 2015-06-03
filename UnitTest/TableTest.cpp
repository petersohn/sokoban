#include "Status/Table.hpp"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(TableTest)

BOOST_AUTO_TEST_CASE(size) {
    std::size_t width = 23;
    std::size_t height = 50;
    Table table{width, height};

    BOOST_CHECK_EQUAL(table.width(), width);
    BOOST_CHECK_EQUAL(table.height(), height);
}

BOOST_AUTO_TEST_CASE(walls) {
    Table table(3, 3);
    table.wall(Point{1, 2}, false);
    table.wall(Point{2, 0}, false);

    BOOST_CHECK_EQUAL(table.wall(Point{0, 0}), true);
    BOOST_CHECK_EQUAL(table.wall(Point{0, 1}), true);
    BOOST_CHECK_EQUAL(table.wall(Point{0, 2}), true);
    BOOST_CHECK_EQUAL(table.wall(Point{1, 0}), true);
    BOOST_CHECK_EQUAL(table.wall(Point{1, 1}), true);
    BOOST_CHECK_EQUAL(table.wall(Point{1, 2}), false);
    BOOST_CHECK_EQUAL(table.wall(Point{2, 0}), false);
    BOOST_CHECK_EQUAL(table.wall(Point{2, 1}), true);
    BOOST_CHECK_EQUAL(table.wall(Point{2, 2}), true);

    table.wall(Point{2, 0}, true);
    table.wall(Point{2, 1}, false);

    BOOST_CHECK_EQUAL(table.wall(Point{0, 0}), true);
    BOOST_CHECK_EQUAL(table.wall(Point{0, 1}), true);
    BOOST_CHECK_EQUAL(table.wall(Point{0, 2}), true);
    BOOST_CHECK_EQUAL(table.wall(Point{1, 0}), true);
    BOOST_CHECK_EQUAL(table.wall(Point{1, 1}), true);
    BOOST_CHECK_EQUAL(table.wall(Point{1, 2}), false);
    BOOST_CHECK_EQUAL(table.wall(Point{2, 0}), true);
    BOOST_CHECK_EQUAL(table.wall(Point{2, 1}), false);
    BOOST_CHECK_EQUAL(table.wall(Point{2, 2}), true);
}

BOOST_AUTO_TEST_CASE(destination) {
    Table table(3, 3);
    table.wall(Point{0, 1}, false);
    table.wall(Point{0, 0}, false);
    table.wall(Point{1, 1}, false);
    table.wall(Point{1, 0}, false);

    Point destination1{1, 0};
    table.destination(destination1);
    BOOST_CHECK_EQUAL(table.destination(), destination1);

    Point destination2{0, 1};
    table.destination(destination2);
    BOOST_CHECK_EQUAL(table.destination(), destination2);
}

BOOST_AUTO_TEST_CASE(startingPoint) {
    Table table(3, 3);
    table.wall(Point{0, 1}, false);
    table.wall(Point{0, 0}, false);
    table.wall(Point{1, 1}, false);
    table.wall(Point{1, 0}, false);

    Point startingPoint1{1, 1};
    table.startingPoint(startingPoint1);
    BOOST_CHECK_EQUAL(table.startingPoint(), startingPoint1);

    Point startingPoint2{1, 0};
    table.startingPoint(startingPoint2);
    BOOST_CHECK_EQUAL(table.startingPoint(), startingPoint2);
}

BOOST_AUTO_TEST_SUITE_END()

