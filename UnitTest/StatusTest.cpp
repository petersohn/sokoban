#include "Status/Status.hpp"
#include "Status/StatusCreator.hpp"

#include <boost/test/unit_test.hpp>

#include <algorithm>
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


BOOST_AUTO_TEST_SUITE_END()

