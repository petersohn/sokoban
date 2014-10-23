#include "MovableChecker.hpp"
#include "Status/StatusCreator.hpp"
#include "MockHeurCalculator.hpp"
#include "CheckerTest.hpp"
#include <boost/test/unit_test.hpp>

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
	CHECK_CHECKER_RESULT(movableCheckerUnderTest, data.second, true);
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
	CHECK_CHECKER_RESULT(movableCheckerUnderTest, data.second, true);
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
	CHECK_CHECKER_RESULT(movableCheckerUnderTest, data.second, true);
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
	CHECK_CHECKER_RESULT(movableCheckerUnderTest, data.second, true);
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
	CHECK_CHECKER_RESULT(movableCheckerUnderTest, data.second, true);
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
	CHECK_CHECKER_RESULT(movableCheckerUnderTest, data.second, true);
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
	CHECK_CHECKER_RESULT(movableCheckerUnderTest, data.second, true);
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
	CHECK_CHECKER_RESULT(movableCheckerUnderTest, data.second, true);
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
	CHECK_CHECKER_RESULT(movableCheckerUnderTest, data.second, true);
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
	CHECK_CHECKER_RESULT(movableCheckerUnderTest, data.second, true);
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
	CHECK_CHECKER_RESULT(movableCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE(not_blocked_when_there_are_multiple_stones_movable_in_one_direction_horizonally)
{
	auto data = createStatus(5, 5, {
			"y....",
			".....",
			".ooo.",
			".....",
			"x...."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{1, 3}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{2, 3}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{3, 3}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(movableCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE(not_blocked_when_there_are_multiple_stones_movable_in_one_direction_vertically)
{
	auto data = createStatus(5, 5, {
			"y....",
			"..o..",
			"..o..",
			"..o..",
			"x...."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{3, 1}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{3, 2}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{3, 3}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(movableCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE(not_blocked_when_there_are_multiple_stones_movable_in_another_direction_horizonally)
{
	auto data = createStatus(5, 5, {
			"y....",
			".....",
			".ooo.",
			".....",
			"x...."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{1, 1}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{2, 1}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{3, 1}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(movableCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE(not_blocked_when_there_are_multiple_stones_movable_in_another_direction_vertically)
{
	auto data = createStatus(5, 5, {
			"y....",
			"..o..",
			"..o..",
			"..o..",
			"x...."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{1, 1}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{1, 2}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{1, 3}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(movableCheckerUnderTest, data.second, true);
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
	CHECK_CHECKER_RESULT(movableCheckerUnderTest, data.second, false);
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
	CHECK_CHECKER_RESULT(movableCheckerUnderTest, data.second, false);
}

BOOST_AUTO_TEST_CASE(blocked_when_a_stone_is_not_movable_vertically_and_blocked_by_stone_horizontally)
{
	auto data = createStatus(5, 5, {
			"y....",
			".....",
			".oo..",
			".....",
			"x...."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{1, 1}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{1, 3}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{2, 1}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{2, 3}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(movableCheckerUnderTest, data.second, false);
}

BOOST_AUTO_TEST_CASE(blocked_when_a_stone_is_not_movable_horizontally_and_blocked_by_stone_vertically)
{
	auto data = createStatus(5, 5, {
			"y....",
			"..o..",
			"..o..",
			".....",
			"x...."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{1, 1}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{3, 1}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{1, 2}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{3, 2}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(movableCheckerUnderTest, data.second, false);
}

BOOST_AUTO_TEST_CASE(blocked_when_there_are_multiple_stones_near_the_wall_vertically)
{
	auto data = createStatus(5, 5, {
			"y....",
			"..o*.",
			"..o*.",
			"...*.",
			"x...."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(movableCheckerUnderTest, data.second, false);
}

BOOST_AUTO_TEST_CASE(blocked_when_there_are_multiple_stones_near_the_wall_horizontally)
{
	auto data = createStatus(5, 5, {
			"y....",
			".***.",
			".oo..",
			".....",
			"x...."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(movableCheckerUnderTest, data.second, false);
}

BOOST_AUTO_TEST_CASE(blocked_when_the_stones_are_blocking_each_other)
{
	auto data = createStatus(5, 5, {
			"y....",
			".....",
			"..oo.",
			"..oo.",
			"x...."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(movableCheckerUnderTest, data.second, false);
}

BOOST_AUTO_TEST_SUITE_END()
