#include "MovableChecker.hpp"
#include "Status/StatusCreator.hpp"
#include <boost/test/unit_test.hpp>
#include <turtle/mock.hpp>

MOCK_BASE_CLASS(MockHeurCalculator, HeurCalculator) {
	MOCK_CONST_METHOD(calculateStatus, 3, int(
			const Status&, const MoveDescriptor*, const std::shared_ptr<Node>&))
	MOCK_CONST_METHOD(calculateStone, 2, int(const Status&, Point))
};

struct MovableCheckerFixture {
	std::shared_ptr<MockHeurCalculator> heurCalculator =
		std::make_shared<MockHeurCalculator>();
	MovableChecker movableCheckerUnderTest{heurCalculator};
};

BOOST_FIXTURE_TEST_SUITE(MovableCheckerTest, MovableCheckerFixture)

BOOST_AUTO_TEST_CASE(not_blocked_when_there_is_a_single_stone_in_the_middle)
{
	auto data = createStatus(5, 5, {
			"y....",
			".....",
			"..o..",
			".....",
			"x...."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	BOOST_CHECK(movableCheckerUnderTest.check(data.second, Point{2, 2}));
}

BOOST_AUTO_TEST_SUITE_END()
