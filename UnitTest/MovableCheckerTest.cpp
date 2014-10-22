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

BOOST_AUTO_TEST_CASE(not_blocked_when_there_are_multiple_stones_in_the_middle_horizontally)
{
	auto data = createStatus(5, 5, {
			"y....",
			".....",
			".ooo.",
			".....",
			"x...."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	BOOST_CHECK(movableCheckerUnderTest.check(data.second, Point{1, 2}));
	BOOST_CHECK(movableCheckerUnderTest.check(data.second, Point{2, 2}));
	BOOST_CHECK(movableCheckerUnderTest.check(data.second, Point{3, 2}));
}

BOOST_AUTO_TEST_CASE(not_blocked_when_there_are_multiple_stones_in_the_middle_vertically)
{
	auto data = createStatus(5, 5, {
			"y....",
			"..o..",
			"..o..",
			"..o..",
			"x...."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	BOOST_CHECK(movableCheckerUnderTest.check(data.second, Point{2, 1}));
	BOOST_CHECK(movableCheckerUnderTest.check(data.second, Point{2, 2}));
	BOOST_CHECK(movableCheckerUnderTest.check(data.second, Point{2, 3}));
}

BOOST_AUTO_TEST_CASE(not_blocked_when_there_are_multiple_stones_in_the_middle_in_different_shapes)
{
	auto data = createStatus(5, 5, {
			"y....",
			".oo..",
			"..o..",
			"..oo.",
			"x...."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	BOOST_CHECK(movableCheckerUnderTest.check(data.second, Point{2, 1}));
	BOOST_CHECK(movableCheckerUnderTest.check(data.second, Point{2, 2}));
	BOOST_CHECK(movableCheckerUnderTest.check(data.second, Point{2, 3}));
	BOOST_CHECK(movableCheckerUnderTest.check(data.second, Point{1, 1}));
	BOOST_CHECK(movableCheckerUnderTest.check(data.second, Point{3, 3}));
}

BOOST_AUTO_TEST_CASE(not_blocked_when_there_is_a_stone_surrounded_by_movable_stones)
{
	auto data = createStatus(5, 5, {
			"y....",
			"..o..",
			".ooo.",
			"..o..",
			"x...."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	BOOST_CHECK(movableCheckerUnderTest.check(data.second, Point{2, 1}));
	BOOST_CHECK(movableCheckerUnderTest.check(data.second, Point{2, 2}));
	BOOST_CHECK(movableCheckerUnderTest.check(data.second, Point{2, 3}));
	BOOST_CHECK(movableCheckerUnderTest.check(data.second, Point{1, 2}));
	BOOST_CHECK(movableCheckerUnderTest.check(data.second, Point{3, 2}));
}

BOOST_AUTO_TEST_CASE(not_blocked_when_there_is_a_stone_surrounded_by_walls_vertically)
{
	auto data = createStatus(5, 5, {
			"y....",
			".*.*.",
			".*o*.",
			".*.*.",
			"x...."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	BOOST_CHECK(movableCheckerUnderTest.check(data.second, Point{2, 2}));
}

BOOST_AUTO_TEST_CASE(not_blocked_when_there_is_a_stone_surrounded_by_walls_horizontally)
{
	auto data = createStatus(5, 5, {
			"y....",
			".***.",
			"..o..",
			".***.",
			"x...."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	BOOST_CHECK(movableCheckerUnderTest.check(data.second, Point{2, 2}));
}

BOOST_AUTO_TEST_CASE(not_blocked_when_there_is_a_stone_surrounded_by_walls_vertically_and_movable_in_one_direction)
{
	auto data = createStatus(5, 5, {
			"y....",
			".*.*.",
			".*o*.",
			".*.*.",
			"x...."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{2, 1}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	BOOST_CHECK(movableCheckerUnderTest.check(data.second, Point{2, 2}));
}

BOOST_AUTO_TEST_CASE(not_blocked_when_there_is_a_stone_surrounded_by_walls_horizontally_and_movable_in_one_direction)
{
	auto data = createStatus(5, 5, {
			"y....",
			".***.",
			"..o..",
			".***.",
			"x...."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{1, 2}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	BOOST_CHECK(movableCheckerUnderTest.check(data.second, Point{2, 2}));
}

BOOST_AUTO_TEST_CASE(not_blocked_when_there_is_a_stone_surrounded_by_walls_vertically_and_movable_in_another_direction)
{
	auto data = createStatus(5, 5, {
			"y....",
			".*.*.",
			".*o*.",
			".*.*.",
			"x...."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{2, 3}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	BOOST_CHECK(movableCheckerUnderTest.check(data.second, Point{2, 2}));
}

BOOST_AUTO_TEST_CASE(not_blocked_when_there_is_a_stone_surrounded_by_walls_horizontally_and_movable_in_another_direction)
{
	auto data = createStatus(5, 5, {
			"y....",
			".***.",
			"..o..",
			".***.",
			"x...."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{3, 2}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	BOOST_CHECK(movableCheckerUnderTest.check(data.second, Point{2, 2}));
}

BOOST_AUTO_TEST_CASE(blocked_when_there_is_a_stone_surrounded_by_walls_vertically_and_movable_in_neither_direction)
{
	auto data = createStatus(5, 5, {
			"y....",
			".*.*.",
			".*o*.",
			".*.*.",
			"x...."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{2, 1}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{2, 3}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	BOOST_CHECK(!movableCheckerUnderTest.check(data.second, Point{2, 2}));
}

BOOST_AUTO_TEST_CASE(blocked_when_there_is_a_stone_surrounded_by_walls_horizontally_and_movable_in_neither_direction)
{
	auto data = createStatus(5, 5, {
			"y....",
			".***.",
			"..o..",
			".***.",
			"x...."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{1, 2}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{3, 2}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	BOOST_CHECK(!movableCheckerUnderTest.check(data.second, Point{2, 2}));
}

BOOST_AUTO_TEST_SUITE_END()
