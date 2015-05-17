#include "intersect.hpp"

#include "util/ContainerWrapper.hpp"

#include <boost/test/unit_test.hpp>

#include <algorithm>

BOOST_AUTO_TEST_SUITE(IntersectTest)

BOOST_AUTO_TEST_CASE(distinct_sets)
{
    std::vector<int> input1{1,5,8,3,2};
    std::vector<int> input2{10, 12, -2};
    std::vector<int> expectedResult;
    auto result = intersect(input1, input2);
    std::sort(result.begin(), result.end());
    BOOST_CHECK_EQUAL(util::wrap(result), util::wrap(expectedResult));
}

BOOST_AUTO_TEST_CASE(some_equal_elements)
{
    std::vector<int> input1{12, 5, 7, 2, -1, -3};
    std::vector<int> input2{0, -4, 5, -1, 1};
    std::vector<int> expectedResult{-1, 5};
    auto result = intersect(input1, input2);
    std::sort(result.begin(), result.end());
    BOOST_CHECK_EQUAL(util::wrap(result), util::wrap(expectedResult));
}

BOOST_AUTO_TEST_CASE(equal_inputs)
{
    std::vector<int> input1{5,2,0,8};
    std::vector<int> input2{5,2,0,8};
    std::vector<int> expectedResult{5,2,0,8};
    auto result = intersect(input1, input2);
    std::sort(result.begin(), result.end());
    std::sort(expectedResult.begin(), expectedResult.end());
    BOOST_CHECK_EQUAL(util::wrap(result), util::wrap(expectedResult));
}

BOOST_AUTO_TEST_SUITE_END()

