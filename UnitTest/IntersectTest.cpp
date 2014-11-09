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

BOOST_AUTO_TEST_SUITE_END()

