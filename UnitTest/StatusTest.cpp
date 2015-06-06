#include "Status/Status.hpp"
#include "Status/StatusCreator.hpp"
#include "ArrayIO.hpp"

#include "CreateTestStatus.hpp"
#include "OperatorChecks.hpp"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/test/unit_test.hpp>

#include <algorithm>
#include <ostream>
#include <vector>

BOOST_AUTO_TEST_SUITE(StatusTest)

BOOST_AUTO_TEST_CASE(consistent_table) {
    std::size_t width = 3;
    std::size_t height = 2;
    auto data = createStatus(width, height, {
            "**x",
            ".oy"});
    const auto& status = data.second;

    BOOST_CHECK_EQUAL(status.width(), width);
    BOOST_CHECK_EQUAL(status.height(), height);
    BOOST_CHECK_EQUAL(&status.table(), data.first.get());
}

BOOST_AUTO_TEST_CASE(consistent_state) {
    std::size_t width = 3;
    std::size_t height = 3;
    auto data = createStatus(width, height, {
            "**x",
            ".o.",
            "o.y"});
    const auto& status = data.second;

    const auto& state = status.state();
    BOOST_CHECK_EQUAL(state.size(), 2);
    std::vector<Point> stateVector(state.begin(), state.end());
    std::sort(stateVector.begin(), stateVector.end());
    BOOST_CHECK_EQUAL(stateVector[0], (Point{1, 1}));
    BOOST_CHECK_EQUAL(stateVector[1], (Point{0, 2}));

    BOOST_CHECK_EQUAL(status.value(Point{0, 0}), FieldType::wall);
    BOOST_CHECK_EQUAL(status.value(Point{1, 0}), FieldType::wall);
    BOOST_CHECK_EQUAL(status.value(Point{2, 0}), FieldType::floor);
    BOOST_CHECK_EQUAL(status.value(Point{0, 1}), FieldType::floor);
    BOOST_CHECK_EQUAL(status.value(Point{1, 1}), FieldType::stone);
    BOOST_CHECK_EQUAL(status.value(Point{2, 1}), FieldType::floor);
    BOOST_CHECK_EQUAL(status.value(Point{0, 2}), FieldType::stone);
    BOOST_CHECK_EQUAL(status.value(Point{1, 2}), FieldType::floor);
    BOOST_CHECK_EQUAL(status.value(Point{2, 2}), FieldType::floor);
}

BOOST_AUTO_TEST_CASE(consistent_current_pos) {
    std::size_t width = 3;
    std::size_t height = 3;
    auto data = createStatus(width, height, {
            "**x",
            ".o.",
            "o.y"});
    const auto& status = data.second;

    BOOST_CHECK_EQUAL(status.currentPos(), (Point{2, 2}));
}

BOOST_AUTO_TEST_CASE(consistent_reachable) {
    std::size_t width = 3;
    std::size_t height = 3;
    auto data = createStatus(width, height, {
            "**x",
            ".o.",
            "o.y"});
    const auto& status = data.second;

    BOOST_CHECK_EQUAL(status.reachable(Point{0, 0}), false);
    BOOST_CHECK_EQUAL(status.reachable(Point{1, 0}), false);
    BOOST_CHECK_EQUAL(status.reachable(Point{2, 0}), true);
    BOOST_CHECK_EQUAL(status.reachable(Point{0, 1}), false);
    BOOST_CHECK_EQUAL(status.reachable(Point{1, 1}), false);
    BOOST_CHECK_EQUAL(status.reachable(Point{2, 1}), true);
    BOOST_CHECK_EQUAL(status.reachable(Point{0, 2}), false);
    BOOST_CHECK_EQUAL(status.reachable(Point{1, 2}), true);
    BOOST_CHECK_EQUAL(status.reachable(Point{2, 2}), true);
}

BOOST_AUTO_TEST_CASE(shiftCurrentPos_leaves_reachability) {
    std::size_t width = 3;
    std::size_t height = 3;
    auto data = createStatus(width, height, {
            "**x",
            ".o.",
            "o.y"});
    auto& status = data.second;

    Point originalCurrentPos = status.currentPos();
    auto originalReachability = status.reachableArray();

    status.shiftCurrentPos();
    BOOST_CHECK_NE(status.currentPos(), originalCurrentPos);
    BOOST_CHECK_EQUAL(status.reachableArray(), originalReachability);
}

BOOST_AUTO_TEST_CASE(equal_statuses_are_equal) {
    std::size_t width = 3;
    std::size_t height = 3;
    auto data = createStatus(width, height, {
            "**x",
            ".o.",
            "o.y"});
    const auto& table = *data.first;
    const auto& status = data.second;

    auto status2 = createTestStatus(table, {
            "**x",
            ".o.",
            "o.y"});

    CHECK_EQ(status, status2);
}

BOOST_AUTO_TEST_CASE(statuses_with_currentPos_in_same_region_are_equal) {
    std::size_t width = 3;
    std::size_t height = 3;
    auto data = createStatus(width, height, {
            "**x",
            ".o.",
            "o.y"});
    const auto& table = *data.first;
    const auto& status = data.second;

    auto status2 = createTestStatus(table, {
            "**x",
            ".oy",
            "o.."});

    CHECK_EQ(status, status2);
}

BOOST_AUTO_TEST_CASE(statuses_with_currentPos_in_different_region_are_different) {
    std::size_t width = 3;
    std::size_t height = 3;
    auto data = createStatus(width, height, {
            "**x",
            ".o.",
            "o.y"});
    const auto& table = *data.first;
    const auto& status = data.second;

    auto status2 = createTestStatus(table, {
            "**x",
            "yo.",
            "o.."});

    CHECK_NE(status, status2);
}

BOOST_AUTO_TEST_CASE(statuses_with_different_stones_are_different) {
    std::size_t width = 3;
    std::size_t height = 3;
    auto data = createStatus(width, height, {
            "**x",
            ".o.",
            "o.y"});
    const auto& table = *data.first;
    const auto& status = data.second;

    auto status2 = createTestStatus(table, {
            "**x",
            ".o.",
            "..y"});

    CHECK_NE(status, status2);
}

BOOST_AUTO_TEST_CASE(statuses_with_stones_on_different_positions_are_different) {
    std::size_t width = 3;
    std::size_t height = 3;
    auto data = createStatus(width, height, {
            "**x",
            ".o.",
            "o.y"});
    const auto& table = *data.first;
    const auto& status = data.second;

    auto status2 = createTestStatus(table, {
            "**x",
            ".o.",
            ".oy"});

    CHECK_NE(status, status2);
}

BOOST_AUTO_TEST_CASE(addStone) {
    std::size_t width = 3;
    std::size_t height = 3;
    auto data = createStatus(width, height, {
            "**x",
            ".o.",
            "o.y"});
    const auto& table = *data.first;
    const auto& status = data.second;

    auto status2 = createTestStatus(table, {
            "**x",
            ".o.",
            "..y"});

    BOOST_CHECK(status2.addStone(Point{0, 2}));
    BOOST_CHECK_EQUAL(status2, status);
}

BOOST_AUTO_TEST_CASE(moveStone) {
    std::size_t width = 3;
    std::size_t height = 3;
    auto data = createStatus(width, height, {
            "**x",
            ".o.",
            "o.y"});
    const auto& table = *data.first;
    const auto& status = data.second;

    auto status2 = createTestStatus(table, {
            "**x",
            ".o.",
            ".oy"});

    BOOST_CHECK(status2.moveStone(Point{1, 2}, Point{0, 2}));
    BOOST_CHECK_EQUAL(status2, status);
}

BOOST_AUTO_TEST_CASE(removeStone) {
    std::size_t width = 3;
    std::size_t height = 3;
    auto data = createStatus(width, height, {
            "**x",
            ".o.",
            "..y"});
    const auto& table = *data.first;
    const auto& status = data.second;

    auto status2 = createTestStatus(table, {
            "**x",
            ".o.",
            "o.y"});

    BOOST_CHECK(status2.removeStone(Point{0, 2}));
    BOOST_CHECK_EQUAL(status2, status);
}

BOOST_AUTO_TEST_CASE(moveStone_out_of_the_table_fails) {
    std::size_t width = 3;
    std::size_t height = 3;
    auto data = createStatus(width, height, {
            "**x",
            ".o.",
            "o.y"});
    auto& status = data.second;

    BOOST_CHECK(!status.moveStone(Point{0, 2}, Point{-1, 2}));
}

BOOST_AUTO_TEST_CASE(moveStone_to_other_stone_fails) {
    std::size_t width = 3;
    std::size_t height = 3;
    auto data = createStatus(width, height, {
            "**x",
            "...",
            "ooy"});
    auto& status = data.second;

    BOOST_CHECK(!status.moveStone(Point{2, 2}, Point{0, 2}));
}

BOOST_AUTO_TEST_CASE(moveStone_into_wall_fails) {
    std::size_t width = 3;
    std::size_t height = 3;
    auto data = createStatus(width, height, {
            "**x",
            ".o.",
            "o.y"});
    auto& status = data.second;

    BOOST_CHECK(!status.moveStone(Point{1, 1}, Point{1, 0}));
}

BOOST_AUTO_TEST_CASE(remove_non_existing_stone_fails) {
    std::size_t width = 3;
    std::size_t height = 3;
    auto data = createStatus(width, height, {
            "**x",
            "...",
            "ooy"});
    auto& status = data.second;

    BOOST_CHECK(!status.removeStone(Point{1, 1}));
}

BOOST_AUTO_TEST_CASE(add_stone_onto_other_stone_fails) {
    std::size_t width = 3;
    std::size_t height = 3;
    auto data = createStatus(width, height, {
            "**x",
            ".o.",
            "o.y"});
    auto& status = data.second;

    BOOST_CHECK(!status.addStone(Point{1, 1}));
}

BOOST_AUTO_TEST_CASE(serialize) {
    std::size_t width = 3;
    std::size_t height = 3;
    auto data = createStatus(width, height, {
            "**x",
            ".o.",
            "o.y"});
    const auto& table = *data.first;
    auto& status1 = data.second;

    std::stringstream stream;

    boost::archive::text_oarchive out{stream};
    out << status1;

    Status status2{table};
    boost::archive::text_iarchive in{stream};
    in >> status2;

    BOOST_CHECK_EQUAL(status2, status1);
}



BOOST_AUTO_TEST_SUITE_END()

