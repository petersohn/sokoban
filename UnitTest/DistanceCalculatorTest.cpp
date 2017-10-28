#include "DistanceCalculator.hpp"
#include "Status/Status.hpp"
#include "Status/StatusCreator.hpp"

#include "MatrixIO.hpp"

#include <boost/test/unit_test.hpp>

using namespace sokoban;

BOOST_AUTO_TEST_SUITE(DistanceCalculatorTest)

BOOST_AUTO_TEST_CASE(from_corner) {
    std::size_t width = 4;
    std::size_t height = 3;

    auto result = calculateDistances(width, height, Point(3, 0),
            [](Point /*p*/, Point /*d*/) { return true; });
    Matrix<int> expectedResult{width, height, {
            3, 2, 1, 0,
            4, 3, 2, 1,
            5, 4, 3, 2}};
    BOOST_CHECK_EQUAL(result, expectedResult);
}

BOOST_AUTO_TEST_CASE(from_middle) {
    std::size_t width = 4;
    std::size_t height = 3;

    auto result = calculateDistances(width, height, Point(2, 1),
            [](Point /*p*/, Point /*d*/) { return true; });
    Matrix<int> expectedResult{width, height, {
            3, 2, 1, 2,
            2, 1, 0, 1,
            3, 2, 1, 2}};
    BOOST_CHECK_EQUAL(result, expectedResult);
}

BOOST_AUTO_TEST_CASE(blocked_point) {
    std::size_t width = 4;
    std::size_t height = 3;

    auto result = calculateDistances(width, height, Point(2, 1),
            [](Point p, Point d) {
                return (p+d) != Point{1, 1};
            });
    Matrix<int> expectedResult{width, height, {
             3,  2,  1,  2,
             4, -1,  0,  1,
             3,  2,  1,  2}};
    BOOST_CHECK_EQUAL(result, expectedResult);
}

BOOST_AUTO_TEST_CASE(walled_off) {
    std::size_t width = 4;
    std::size_t height = 3;

    auto result = calculateDistances(width, height, Point(2, 1),
            [](Point p, Point d) {
                return (p+d).x != 1;
            });
    Matrix<int> expectedResult{width, height, {
            -1, -1,  1,  2,
            -1, -1,  0,  1,
            -1, -1,  1,  2}};
    BOOST_CHECK_EQUAL(result, expectedResult);
}

BOOST_AUTO_TEST_CASE(blocked_direction) {
    std::size_t width = 4;
    std::size_t height = 3;

    auto result = calculateDistances(width, height, Point(2, 1),
            [](Point p, Point d) {
                return (p+d) != Point{1, 1} && d.y != 1;
            });
    Matrix<int> expectedResult{width, height, {
             3,  2,  1,  2,
            -1, -1,  0,  1,
            -1, -1, -1, -1}};
    BOOST_CHECK_EQUAL(result, expectedResult);
}

BOOST_AUTO_TEST_SUITE_END()

