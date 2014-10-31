#include "Status/floodFill.hpp"
#include "Status/StatusCreator.hpp"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(MovableCheckerTest)

BOOST_AUTO_TEST_SUITE(reachability)

BOOST_AUTO_TEST_CASE(floors_are_reachable_but_walls_and_stones_are_not)
{
	std::size_t width = 4;
	std::size_t height = 2;
	Array<bool> result{width, height};
	auto data = createStatus(width, height, {
			"x...",
			"y*o."
		});

	floodFill(data.second, Point{1, 0}, result);
	BOOST_CHECK_EQUAL(result[Point(0, 0)], true);
	BOOST_CHECK_EQUAL(result[Point(1, 0)], true);
	BOOST_CHECK_EQUAL(result[Point(2, 0)], true);
	BOOST_CHECK_EQUAL(result[Point(3, 0)], true);
	BOOST_CHECK_EQUAL(result[Point(0, 1)], true);
	BOOST_CHECK_EQUAL(result[Point(1, 1)], false);
	BOOST_CHECK_EQUAL(result[Point(2, 1)], false);
	BOOST_CHECK_EQUAL(result[Point(3, 1)], true);
}

BOOST_AUTO_TEST_CASE(walls_and_stones_block_reachability)
{
	std::size_t width = 4;
	std::size_t height = 2;
	Array<bool> result{width, height};
	auto data = createStatus(width, height, {
			"x.*.",
			"y.o."
		});

	floodFill(data.second, Point{0, 0}, result);
	BOOST_CHECK_EQUAL(result[Point(0, 0)], true);
	BOOST_CHECK_EQUAL(result[Point(1, 0)], true);
	BOOST_CHECK_EQUAL(result[Point(2, 0)], false);
	BOOST_CHECK_EQUAL(result[Point(3, 0)], false);
	BOOST_CHECK_EQUAL(result[Point(0, 1)], true);
	BOOST_CHECK_EQUAL(result[Point(1, 1)], true);
	BOOST_CHECK_EQUAL(result[Point(2, 1)], false);
	BOOST_CHECK_EQUAL(result[Point(3, 1)], false);

	floodFill(data.second, Point{3, 1}, result);
	BOOST_CHECK_EQUAL(result[Point(0, 0)], false);
	BOOST_CHECK_EQUAL(result[Point(1, 0)], false);
	BOOST_CHECK_EQUAL(result[Point(2, 0)], false);
	BOOST_CHECK_EQUAL(result[Point(3, 0)], true);
	BOOST_CHECK_EQUAL(result[Point(0, 1)], false);
	BOOST_CHECK_EQUAL(result[Point(1, 1)], false);
	BOOST_CHECK_EQUAL(result[Point(2, 1)], false);
	BOOST_CHECK_EQUAL(result[Point(3, 1)], true);
}

BOOST_AUTO_TEST_CASE(walls_and_stones_block_reachability_diagonally_1)
{
	std::size_t width = 4;
	std::size_t height = 2;
	Array<bool> result{width, height};
	auto data = createStatus(width, height, {
			"x.*.",
			"yo.."
		});

	floodFill(data.second, Point{0, 0}, result);
	BOOST_CHECK_EQUAL(result[Point(0, 0)], true);
	BOOST_CHECK_EQUAL(result[Point(1, 0)], true);
	BOOST_CHECK_EQUAL(result[Point(2, 0)], false);
	BOOST_CHECK_EQUAL(result[Point(3, 0)], false);
	BOOST_CHECK_EQUAL(result[Point(0, 1)], true);
	BOOST_CHECK_EQUAL(result[Point(1, 1)], false);
	BOOST_CHECK_EQUAL(result[Point(2, 1)], false);
	BOOST_CHECK_EQUAL(result[Point(3, 1)], false);

	floodFill(data.second, Point{3, 1}, result);
	BOOST_CHECK_EQUAL(result[Point(0, 0)], false);
	BOOST_CHECK_EQUAL(result[Point(1, 0)], false);
	BOOST_CHECK_EQUAL(result[Point(2, 0)], false);
	BOOST_CHECK_EQUAL(result[Point(3, 0)], true);
	BOOST_CHECK_EQUAL(result[Point(0, 1)], false);
	BOOST_CHECK_EQUAL(result[Point(1, 1)], false);
	BOOST_CHECK_EQUAL(result[Point(2, 1)], true);
	BOOST_CHECK_EQUAL(result[Point(3, 1)], true);
}

BOOST_AUTO_TEST_CASE(walls_and_stones_block_reachability_diagonally_2)
{
	std::size_t width = 4;
	std::size_t height = 2;
	Array<bool> result{width, height};
	auto data = createStatus(width, height, {
			"x*..",
			"y.o."
		});

	floodFill(data.second, Point{0, 0}, result);
	BOOST_CHECK_EQUAL(result[Point(0, 0)], true);
	BOOST_CHECK_EQUAL(result[Point(1, 0)], false);
	BOOST_CHECK_EQUAL(result[Point(2, 0)], false);
	BOOST_CHECK_EQUAL(result[Point(3, 0)], false);
	BOOST_CHECK_EQUAL(result[Point(0, 1)], true);
	BOOST_CHECK_EQUAL(result[Point(1, 1)], true);
	BOOST_CHECK_EQUAL(result[Point(2, 1)], false);
	BOOST_CHECK_EQUAL(result[Point(3, 1)], false);

	floodFill(data.second, Point{3, 1}, result);
	BOOST_CHECK_EQUAL(result[Point(0, 0)], false);
	BOOST_CHECK_EQUAL(result[Point(1, 0)], false);
	BOOST_CHECK_EQUAL(result[Point(2, 0)], true);
	BOOST_CHECK_EQUAL(result[Point(3, 0)], true);
	BOOST_CHECK_EQUAL(result[Point(0, 1)], false);
	BOOST_CHECK_EQUAL(result[Point(1, 1)], false);
	BOOST_CHECK_EQUAL(result[Point(2, 1)], false);
	BOOST_CHECK_EQUAL(result[Point(3, 1)], true);
}

BOOST_AUTO_TEST_SUITE_END() // reachability

BOOST_AUTO_TEST_SUITE(border)

#define CHECK_BORDER_INCLUDES(border, stone) \
		BOOST_CHECK(::std::find((border).begin(), (border).end(), (stone)) != \
				(border).end())

BOOST_AUTO_TEST_CASE(single_stone_in_the_middle)
{
	std::size_t width = 4;
	std::size_t height = 3;
	Array<bool> result{width, height};
	auto data = createStatus(width, height, {
			"x...",
			"y.o."
			"...."
		});

	Status::BorderType border;
	floodFill(data.second, Point{1, 0}, result, border);
	BOOST_CHECK_EQUAL(border.size(), 1);
	CHECK_BORDER_INCLUDES(border, (Point{2, 1}));

}

BOOST_AUTO_TEST_SUITE_END() // border

BOOST_AUTO_TEST_SUITE_END() // MovableCheckerTest

