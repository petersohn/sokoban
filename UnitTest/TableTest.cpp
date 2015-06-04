#include "Status/Table.hpp"
#include "OperatorChecks.hpp"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/test/unit_test.hpp>

#include <sstream>

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

BOOST_AUTO_TEST_CASE(equal_tables_are_equal) {
    Table table1(3, 3);
    table1.wall(Point{0, 0}, false);
    table1.wall(Point{0, 1}, false);
    table1.wall(Point{0, 2}, false);
    table1.wall(Point{1, 1}, false);
    table1.startingPoint(Point{1, 0});
    table1.destination(Point{1, 1});

    Table table2(3, 3);
    table2.wall(Point{0, 0}, false);
    table2.wall(Point{0, 1}, false);
    table2.wall(Point{0, 2}, false);
    table2.wall(Point{1, 1}, false);
    table2.destination(Point{1, 1});
    table2.startingPoint(Point{1, 0});

    CHECK_EQ(table1, table2);
}

BOOST_AUTO_TEST_CASE(copied_tables_are_same) {
    Table table1(3, 3);
    table1.wall(Point{0, 0}, false);
    table1.wall(Point{0, 1}, false);
    table1.wall(Point{0, 2}, false);
    table1.wall(Point{1, 1}, false);
    table1.startingPoint(Point{1, 0});
    table1.destination(Point{1, 1});

    Table table2 = table1;

    CHECK_EQ(table1, table2);
}

BOOST_AUTO_TEST_CASE(differently_sized_tables_are_different) {
    Table table1(3, 3);
    Table table2(3, 5);

    CHECK_NE(table1, table2);
}

BOOST_AUTO_TEST_CASE(tables_with_different_walls_are_different) {
    Table table1(3, 3);
    table1.wall(Point{0, 0}, false);
    table1.wall(Point{0, 1}, false);
    table1.wall(Point{0, 2}, false);
    table1.wall(Point{1, 1}, false);
    table1.startingPoint(Point{1, 0});
    table1.destination(Point{1, 1});

    Table table2(3, 3);
    table2.wall(Point{0, 0}, false);
    table2.wall(Point{0, 1}, false);
    table2.wall(Point{1, 1}, false);
    table2.destination(Point{1, 1});
    table2.startingPoint(Point{1, 0});

    CHECK_NE(table1, table2);
}

BOOST_AUTO_TEST_CASE(tables_with_different_starting_points_are_different) {
    Table table1(3, 3);
    table1.wall(Point{0, 0}, false);
    table1.wall(Point{0, 1}, false);
    table1.wall(Point{0, 2}, false);
    table1.wall(Point{1, 1}, false);
    table1.startingPoint(Point{1, 0});
    table1.destination(Point{1, 1});

    Table table2(3, 3);
    table2.wall(Point{0, 0}, false);
    table2.wall(Point{0, 1}, false);
    table2.wall(Point{0, 2}, false);
    table2.wall(Point{1, 1}, false);
    table2.destination(Point{1, 1});
    table2.startingPoint(Point{0, 0});

    CHECK_NE(table1, table2);
}

BOOST_AUTO_TEST_CASE(tables_with_different_destinations_are_different) {
    Table table1(3, 3);
    table1.wall(Point{0, 0}, false);
    table1.wall(Point{0, 1}, false);
    table1.wall(Point{0, 2}, false);
    table1.wall(Point{1, 1}, false);
    table1.startingPoint(Point{1, 0});
    table1.destination(Point{1, 1});

    Table table2(3, 3);
    table2.wall(Point{0, 0}, false);
    table2.wall(Point{0, 1}, false);
    table2.wall(Point{0, 2}, false);
    table2.wall(Point{1, 1}, false);
    table2.destination(Point{1, 2});
    table2.startingPoint(Point{1, 0});

    CHECK_NE(table1, table2);
}

BOOST_AUTO_TEST_CASE(serialize_works) {
    Table table1(3, 3);
    table1.wall(Point{0, 0}, false);
    table1.wall(Point{0, 1}, false);
    table1.wall(Point{0, 2}, false);
    table1.wall(Point{1, 1}, false);
    table1.startingPoint(Point{1, 0});
    table1.destination(Point{1, 1});

    std::stringstream stream;
    boost::archive::text_oarchive out{stream};
    out << table1;

    Table table2{0, 0};
    boost::archive::text_iarchive in{stream};
    in >> table2;

    CHECK_EQ(table1, table2);
}

BOOST_AUTO_TEST_SUITE_END()

