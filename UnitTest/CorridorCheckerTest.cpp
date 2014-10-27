#include "CorridorChecker.hpp"
#include "Status/StatusCreator.hpp"
#include "MockHeurCalculator.hpp"
#include "Status/Status.hpp"
#include "CheckerTest.hpp"
#include <boost/test/unit_test.hpp>

struct CorridorCheckerFixture {
	std::shared_ptr<MockHeurCalculator> heurCalculator =
		std::make_shared<MockHeurCalculator>();
	CorridorChecker corridorCheckerUnderTest{heurCalculator};
};

BOOST_FIXTURE_TEST_SUITE(CorridorCheckerTest, CorridorCheckerFixture)

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

BOOST_AUTO_TEST_CASE(not_blocked_when_horizontal_corridor_can_be_opened_from_all_directions)
{
	auto data = createStatus(5, 5, {
			"y....",
			".ooo.",
			"o...o",
			".ooo.",
			"x...."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE(not_blocked_when_horizontal_corridor_can_be_opened_from_one_direction)
{
	auto data = createStatus(5, 5, {
			"y....",
			".ooo.",
			"o...o",
			".oooo",
			"x...."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE(not_blocked_when_horizontal_corridor_can_be_opened_from_another_direction)
{
	auto data = createStatus(5, 5, {
			"y....",
			"oooo.",
			"o...o",
			".ooo.",
			"x...."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE(blocked_when_horizontal_corridor_cannot_be_opened1)
{
	auto data = createStatus(5, 5, {
			"y....",
			"ooooo",
			"o...o",
			".ooo.",
			"x...."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(corridorCheckerUnderTest, data.second, false);
}

BOOST_AUTO_TEST_CASE(blocked_when_horizontal_corridor_cannot_be_opened2)
{
	auto data = createStatus(5, 5, {
			"y....",
			".oooo",
			"o...o",
			"oooo.",
			"x...."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(corridorCheckerUnderTest, data.second, false);
}

BOOST_AUTO_TEST_CASE(not_blocked_when_vertical_corridor_can_be_opened_from_all_directions)
{
	auto data = createStatus(5, 5, {
			"y.o..",
			".o.o.",
			".o.o.",
			".o.o.",
			"x.o.."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE(not_blocked_when_vertical_corridor_can_be_opened_from_one_direction)
{
	auto data = createStatus(5, 5, {
			"y.o..",
			".o.o.",
			".o.o.",
			".o.o.",
			"x.oo."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE(not_blocked_when_vertical_corridor_can_be_opened_from_another_direction)
{
	auto data = createStatus(5, 5, {
			"yoo..",
			".o.o.",
			".o.o.",
			".o.o.",
			"x.o.."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(corridorCheckerUnderTest, data.second, true);
}

BOOST_AUTO_TEST_CASE(blocked_when_vertical_corridor_cannot_be_opened1)
{
	auto data = createStatus(5, 5, {
			"yoo..",
			".o.o.",
			".o.o.",
			".o.o.",
			"xoo.."
		});
	MOCK_EXPECT(heurCalculator->calculateStone).returns(1);
	CHECK_CHECKER_RESULT(corridorCheckerUnderTest, data.second, false);
}

BOOST_AUTO_TEST_CASE(blocked_when_vertical_corridor_cannot_be_opened2)
{
	auto data = createStatus(5, 5, {
			"yoo..",
			".o.o.",
			".o.o.",
			".o.o.",
			"x.oo."
		});
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



BOOST_AUTO_TEST_SUITE_END()

