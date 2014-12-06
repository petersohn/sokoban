#include "ExtendedCorridorChecker.hpp"
#include "Status/StatusCreator.hpp"
#include "MockHeurCalculator.hpp"
#include "Status/Status.hpp"
#include "CheckerTest.hpp"
#include <boost/test/unit_test.hpp>

struct ExtendedCorridorCheckerFixture {
	std::shared_ptr<MockHeurCalculator> heurCalculator =
		std::make_shared<MockHeurCalculator>();
	ExtendedCorridorChecker corridorCheckerUnderTest{heurCalculator};
};

BOOST_FIXTURE_TEST_SUITE(ExtendedCorridorCheckerTest, ExtendedCorridorCheckerFixture)

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
	CHECK_CHECKER_RESULT(corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE(not_blocked_when_1x1_corridor_can_be_opened_from_all_directions)
{
	auto data = createStatus(5, 5, {
			"y....",
			"..o..",
			".o.o.",
			"..o..",
			"x...."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE(not_blocked_when_1x1_corridor_can_be_opened_from_two_directions1)
{
	auto data = createStatus(5, 5, {
			"y....",
			"..oo.",
			".o.o.",
			"..o..",
			"x...."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE(not_blocked_when_1x1_corridor_can_be_opened_from_two_directions2)
{
	auto data = createStatus(5, 5, {
			"y....",
			"..o..",
			".o.o.",
			"..oo.",
			"x...."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE(not_blocked_when_1x1_corridor_can_be_opened_from_two_directions3)
{
	auto data = createStatus(5, 5, {
			"y....",
			"..o..",
			".o.o.",
			".oo..",
			"x...."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE(not_blocked_when_1x1_corridor_can_be_opened_from_two_directions4)
{
	auto data = createStatus(5, 5, {
			"y....",
			".oo..",
			".o.o.",
			"..o..",
			"x...."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE(not_blocked_when_1x1_corridor_can_be_opened_from_one_direction1)
{
	auto data = createStatus(5, 5, {
			"y....",
			".ooo.",
			".o.o.",
			"..o..",
			"x...."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE(not_blocked_when_1x1_corridor_can_be_opened_from_one_direction2)
{
	auto data = createStatus(5, 5, {
			"y....",
			"..oo.",
			".o.o.",
			"..oo.",
			"x...."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE(not_blocked_when_1x1_corridor_can_be_opened_from_one_direction3)
{
	auto data = createStatus(5, 5, {
			"y....",
			"..o..",
			".o.o.",
			".ooo.",
			"x...."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE(not_blocked_when_1x1_corridor_can_be_opened_from_one_direction4)
{
	auto data = createStatus(5, 5, {
			"y....",
			".oo..",
			".o.o.",
			".oo..",
			"x...."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE(blocked_when_1x1_corridor_cannot_be_opened1)
{
	auto data = createStatus(5, 5, {
			"y....",
			"..oo.",
			".o.o.",
			".oo..",
			"x...."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(corridorCheckerUnderTest, data.second, false);
}

BOOST_AUTO_TEST_CASE(blocked_when_1x1_corridor_cannot_be_opened2)
{
	auto data = createStatus(5, 5, {
			"y....",
			".oo..",
			".o.o.",
			"..oo.",
			"x...."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(corridorCheckerUnderTest, data.second, false);
}

BOOST_AUTO_TEST_CASE(not_blocked_when_openings_are_movable_from_one_direction)
{
	auto data = createStatus(5, 5, {
			"y....",
			"..o..",
			".o.o.",
			"..o..",
			"x...."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{1, 1}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{3, 3}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE(not_blocked_when_openings_are_movable_from_another_direction)
{
	auto data = createStatus(5, 5, {
			"y....",
			"..o..",
			".o.o.",
			"..o..",
			"x...."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{1, 3}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{3, 1}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE(blocked_when_openings_are_not_movable)
{
	auto data = createStatus(5, 5, {
			"y....",
			"..o..",
			".o.o.",
			"..o..",
			"x...."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{1, 1}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{1, 3}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{3, 1}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{3, 3}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(corridorCheckerUnderTest, data.second, false);
}

BOOST_AUTO_TEST_CASE(not_blocked_when_horizontal_corridor_can_be_opened_from_all_directions)
{
	auto data = createStatus(7, 7, {
			".......",
			".y.....",
			"..ooo..",
			".o...o.",
			"..ooo..",
			".x.....",
			"......."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE(not_blocked_when_horizontal_corridor_can_be_opened_from_one_direction)
{
	auto data = createStatus(7, 7, {
			".......",
			".y.....",
			"..ooo..",
			".o...o.",
			"..oooo.",
			".x.....",
			"......."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE(not_blocked_when_horizontal_corridor_can_be_opened_from_another_direction)
{
	auto data = createStatus(7, 7, {
			".......",
			".y.....",
			".oooo..",
			".o...o.",
			"..ooo..",
			".x.....",
			"......."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE(blocked_when_horizontal_corridor_cannot_be_opened1)
{
	auto data = createStatus(7, 7, {
			".......",
			".y.....",
			".ooooo.",
			".o...o.",
			"..ooo..",
			".x.....",
			"......."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(corridorCheckerUnderTest, data.second, false);
}

BOOST_AUTO_TEST_CASE(blocked_when_horizontal_corridor_cannot_be_opened2)
{
	auto data = createStatus(7, 7, {
			".......",
			".y.....",
			"..oooo.",
			".o...o.",
			".oooo..",
			".x.....",
			"......."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(corridorCheckerUnderTest, data.second, false);
}

BOOST_AUTO_TEST_CASE(not_blocked_when_openings_are_movable_from_one_direction_horizontally)
{
	auto data = createStatus(7, 7, {
			".......",
			".y.....",
			"..ooo..",
			".o...o.",
			"..ooo..",
			".x.....",
			"......."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{1, 2}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{1, 4}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{5, 4}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE(not_blocked_when_openings_are_movable_from_other_direction_horizontally)
{
	auto data = createStatus(7, 7, {
			".......",
			".y.....",
			"..ooo..",
			".o...o.",
			"..ooo..",
			".x.....",
			"......."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{1, 2}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{5, 2}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{5, 4}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE(blocked_when_openings_are_not_movable_horizontally)
{
	auto data = createStatus(7, 7, {
			".......",
			".y.....",
			"..ooo..",
			".o...o.",
			"..ooo..",
			".x.....",
			"......."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{1, 2}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{1, 4}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{5, 2}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{5, 4}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(corridorCheckerUnderTest, data.second, false);
}

BOOST_AUTO_TEST_CASE(not_blocked_when_vertical_corridor_can_be_opened_from_all_directions)
{
	auto data = createStatus(7, 7, {
			".......",
			".y.o...",
			"..o.o..",
			"..o.o..",
			"..o.o..",
			".x.o...",
			"......."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE(not_blocked_when_vertical_corridor_can_be_opened_from_one_direction)
{
	auto data = createStatus(7, 7, {
			".......",
			".y.o...",
			"..o.o..",
			"..o.o..",
			"..o.o..",
			".x.oo..",
			"......."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE(not_blocked_when_vertical_corridor_can_be_opened_from_another_direction)
{
	auto data = createStatus(7, 7, {
			".......",
			".yoo...",
			"..o.o..",
			"..o.o..",
			"..o.o..",
			".x.o...",
			"......."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE(blocked_when_vertical_corridor_cannot_be_opened1)
{
	auto data = createStatus(7, 7, {
			".......",
			".yoo...",
			"..o.o..",
			"..o.o..",
			"..o.o..",
			".xoo...",
			"......."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(corridorCheckerUnderTest, data.second, false);
}

BOOST_AUTO_TEST_CASE(blocked_when_vertical_corridor_cannot_be_opened2)
{
	auto data = createStatus(7, 7, {
			".......",
			".yoo...",
			"..o.o..",
			"..o.o..",
			"..o.o..",
			".x.oo..",
			"......."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(corridorCheckerUnderTest, data.second, false);
}

BOOST_AUTO_TEST_CASE(not_blocked_when_openings_are_movable_from_one_direction_vertically)
{
	auto data = createStatus(7, 7, {
			".......",
			".y.o...",
			"..o.o..",
			"..o.o..",
			"..o.o..",
			".x.o...",
			"......."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{2, 1}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{4, 1}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{4, 5}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE(not_blocked_when_openings_are_movable_from_other_direction_vertically)
{
	auto data = createStatus(7, 7, {
			".......",
			".y.o...",
			"..o.o..",
			"..o.o..",
			"..o.o..",
			".x.o...",
			"......."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{2, 1}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{2, 5}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{4, 5}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE(blocked_when_openings_are_not_movable_vertically)
{
	auto data = createStatus(7, 7, {
			".......",
			".y.o...",
			"..o.o..",
			"..o.o..",
			"..o.o..",
			".x.o...",
			"......."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{2, 1}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{4, 1}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{2, 5}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{4, 5}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(corridorCheckerUnderTest, data.second, false);
}

BOOST_AUTO_TEST_CASE(not_blocked_when_corridor_can_be_opened_against_wall1)
{
	auto data = createStatus(5, 5, {
			"y....",
			"...oo",
			"..o..",
			"...oo",
			"x...."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE(not_blocked_when_corridor_can_be_opened_against_wall2)
{
	auto data = createStatus(5, 5, {
			"y....",
			".....",
			"..o..",
			".o.o.",
			"xo.o."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE(not_blocked_when_corridor_can_be_opened_against_wall3)
{
	auto data = createStatus(5, 5, {
			"y....",
			"oo...",
			"..o..",
			"oo...",
			"x...."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE(not_blocked_when_corridor_can_be_opened_against_wall4)
{
	auto data = createStatus(5, 5, {
			"yo.o.",
			".o.o.",
			"..o..",
			".....",
			"x...."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE(blocked_when_corridor_cannot_be_opened_against_wall1)
{
	auto data = createStatus(5, 5, {
			"y....",
			"..ooo",
			"..o..",
			"...oo",
			"x...."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(corridorCheckerUnderTest, data.second, false);
}

BOOST_AUTO_TEST_CASE(blocked_when_corridor_cannot_be_opened_against_wall2)
{
	auto data = createStatus(5, 5, {
			"y....",
			".....",
			".oo..",
			".o.o.",
			"xo.o."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(corridorCheckerUnderTest, data.second, false);
}

BOOST_AUTO_TEST_CASE(blocked_when_corridor_cannot_be_opened_against_wall3)
{
	auto data = createStatus(5, 5, {
			"y....",
			"ooo..",
			"..o..",
			"oo...",
			"x...."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(corridorCheckerUnderTest, data.second, false);
}

BOOST_AUTO_TEST_CASE(blocked_when_corridor_cannot_be_opened_against_wall4)
{
	auto data = createStatus(5, 5, {
			"yo.o.",
			".o.o.",
			"..oo.",
			".....",
			"x...."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(corridorCheckerUnderTest, data.second, false);
}

BOOST_AUTO_TEST_CASE(not_blocked_when_opening_target_points_are_not_reachable_horizontally)
{
	auto data = createStatus(5, 5, {
			"y....",
			".ooo.",
			"o...o",
			".ooo.",
			"x...."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{0, 1}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{4, 1}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE(not_blocked_when_opening_target_points_are_not_reachable_vertically)
{
	auto data = createStatus(5, 5, {
			"y.o..",
			".o.o.",
			".o.o.",
			".o.o.",
			"x.o.."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{1, 0}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{1, 4}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE(blocked_when_opening_source_points_are_not_reachable_horizontally)
{
	auto data = createStatus(5, 5, {
			"y....",
			".ooo.",
			"o...o",
			".ooo.",
			"x...."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{0, 3}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{4, 3}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(corridorCheckerUnderTest, data.second, false);
}

BOOST_AUTO_TEST_CASE(blocked_when_opening_source_points_are_not_reachable_vertically)
{
	auto data = createStatus(5, 5, {
			"y.o..",
			".o.o.",
			".o.o.",
			".o.o.",
			"x.o.."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{3, 0}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).with(mock::any, Point{3, 4}).returns(-1);
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(corridorCheckerUnderTest, data.second, false);
}



BOOST_AUTO_TEST_SUITE_END()

