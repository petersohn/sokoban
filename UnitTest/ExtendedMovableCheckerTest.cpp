#include "ExtendedMovableChecker.hpp"
#include "Status/StatusCreator.hpp"
#include "MockHeurCalculator.hpp"
#include "CheckerTest.hpp"
#include <boost/test/unit_test.hpp>

struct ExtendedMovableCheckerFixture {
	std::shared_ptr<MockHeurCalculator> heurCalculator =
		std::make_shared<MockHeurCalculator>();
	ExtendedMovableChecker movableCheckerUnderTest{heurCalculator};
};

BOOST_FIXTURE_TEST_SUITE(ExtendedMovableCheckerTest, ExtendedMovableCheckerFixture)

BOOST_AUTO_TEST_CASE(not_movable_when_source_point_not_reachable_vertically)
{
	auto data = createStatus(5, 5, {
			"y....",
			".....",
			".....",
			"..oo*",
			"x.*.."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{3, 4}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(movableCheckerUnderTest, data.second, false);
}

BOOST_AUTO_TEST_CASE(not_movable_when_source_point_not_reachable_horizontally)
{
	auto data = createStatus(5, 5, {
			"y....",
			".....",
			"...o*",
			"...o.",
			"x..*."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{4, 3}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(movableCheckerUnderTest, data.second, false);
}

BOOST_AUTO_TEST_SUITE_END()
