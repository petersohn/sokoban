#include "ChokePointFinder.hpp"
#include "util/ContainerWrapper.hpp"
#include "Status/StatusCreator.hpp"
#include "Status/Status.hpp"
#include "ComplexChecker.hpp"
#include "MockHeurCalculator.hpp"
#include "MockChecker.hpp"
#include "ArrayIO.hpp"
#include <boost/test/unit_test.hpp>
#include <algorithm>

struct ChokePointFinderTestFixture {
	Options options;
	std::shared_ptr<MockHeurCalculator> heurCalculator =
		std::make_shared<MockHeurCalculator>();
	std::shared_ptr<MockChecker> checker =
		std::make_shared<MockChecker>();

	ChokePointFinderTestFixture()
	{
		MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
		MOCK_EXPECT(heurCalculator->calculateStatus).returns(1);
		MOCK_EXPECT(checker->check).returns(true);
	}

	Array<bool> createChokePointArray(const Table& table,
			const std::vector<Point>& chokePointList)
	{
		Array<bool> result{table.width(), table.height(), false};

		for (Point p: chokePointList) {
			result[p] = true;
		}

		return result;
	}
};


BOOST_FIXTURE_TEST_SUITE(ChokePointFinderTest, ChokePointFinderTestFixture)

BOOST_AUTO_TEST_CASE(no_choke_point_in_empty_table)
{
	options.chokePointNum_ = 1;

	auto data = createStatus(5, 5, {
			"yo...",
			".....",
			".....",
			".....",
			"x...."
		});

	auto chokePoints = findChokePoints(*data.first, options, heurCalculator,
			ComplexChecker{checker}, false);

	BOOST_CHECK_EQUAL(chokePoints, createChokePointArray(*data.first, {}));
}

BOOST_AUTO_TEST_CASE(single_choke_point)
{
	options.chokePointNum_ = 1;

	auto data = createStatus(5, 5, {
			"yo...",
			".....",
			"**.**",
			".....",
			"x...."
		});

	std::vector<Point> expectedResult{Point{2, 1}, Point{2, 2}, Point{2, 3}};
	auto chokePoints = findChokePoints(*data.first, options, heurCalculator,
			ComplexChecker{checker}, false);

	BOOST_CHECK_EQUAL(chokePoints,
			createChokePointArray(*data.first, expectedResult));
}

BOOST_AUTO_TEST_CASE(multiple_choke_points)
{
	options.chokePointNum_ = 1;

	auto data = createStatus(5, 8, {
			"yo...",
			".....",
			"*.***",
			".....",
			".....",
			"***.*",
			".....",
			"x...."
		});

	std::vector<Point> expectedResult{
			Point{1, 1}, Point{1, 2}, Point{1, 3},
			Point{3, 4}, Point{3, 5}, Point{3, 6},
			};
	auto chokePoints = findChokePoints(*data.first, options, heurCalculator,
			ComplexChecker{checker}, false);

	BOOST_CHECK_EQUAL(chokePoints,
			createChokePointArray(*data.first, expectedResult));
}

BOOST_AUTO_TEST_CASE(no_choke_points_if_more_stones_are_needed)
{
	options.chokePointNum_ = 1;

	auto data = createStatus(5, 5, {
			"yo...",
			".....",
			"*.*.*",
			".....",
			"x...."
		});

	auto chokePoints = findChokePoints(*data.first, options, heurCalculator,
			ComplexChecker{checker}, false);

	BOOST_CHECK_EQUAL(chokePoints, createChokePointArray(*data.first, {}));
}

BOOST_AUTO_TEST_CASE(more_choke_points_in_options)
{
	options.chokePointNum_ = 2;

	auto data = createStatus(5, 5, {
			"yo...",
			".....",
			"*.*.*",
			".....",
			"x...."
		});

	std::vector<Point> expectedResult{
			Point{1, 1}, Point{3, 1},
			Point{1, 2}, Point{3, 2},
			Point{1, 3}, Point{3, 3},
			};
	auto chokePoints = findChokePoints(*data.first, options, heurCalculator,
			ComplexChecker{checker}, false);

	BOOST_CHECK_EQUAL(chokePoints,
			createChokePointArray(*data.first, expectedResult));
}

BOOST_AUTO_TEST_CASE(single_choke_point_with_more_points_in_options)
{
	options.chokePointNum_ = 2;

	auto data = createStatus(5, 5, {
			"yo...",
			".....",
			"**.**",
			".....",
			"x...."
		});

	std::vector<Point> expectedResult{Point{2, 1}, Point{2, 2}, Point{2, 3}};
	auto chokePoints = findChokePoints(*data.first, options, heurCalculator,
			ComplexChecker{checker}, false);

	BOOST_CHECK_EQUAL(chokePoints,
			createChokePointArray(*data.first, expectedResult));
}

BOOST_AUTO_TEST_SUITE_END()

