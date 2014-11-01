#include "Status/floodFill.hpp"
#include "Status/StatusCreator.hpp"
#include "ArrayIO.hpp"
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

BOOST_AUTO_TEST_CASE(multiple_reachable_stones)
{
	std::size_t width = 4;
	std::size_t height = 3;
	Array<bool> result{width, height};
	auto data = createStatus(width, height, {
			"x...",
			"y.o.",
			".o.o"
		});

	Status::BorderType border;
	floodFill(data.second, Point{1, 0}, result, border);
	BOOST_CHECK_EQUAL(border.size(), 3);
	CHECK_BORDER_INCLUDES(border, (Point{2, 1}));
	CHECK_BORDER_INCLUDES(border, (Point{1, 2}));
	CHECK_BORDER_INCLUDES(border, (Point{3, 2}));
}

BOOST_AUTO_TEST_CASE(one_stone_is_blocked_by_other_stones)
{
	std::size_t width = 4;
	std::size_t height = 3;
	Array<bool> result{width, height};
	auto data = createStatus(width, height, {
			"x..o",
			"y.o.",
			".o.o"
		});

	Status::BorderType border;
	floodFill(data.second, Point{1, 0}, result, border);
	BOOST_CHECK_EQUAL(border.size(), 3);
	CHECK_BORDER_INCLUDES(border, (Point{3, 0}));
	CHECK_BORDER_INCLUDES(border, (Point{1, 2}));
	CHECK_BORDER_INCLUDES(border, (Point{2, 1}));
}

BOOST_AUTO_TEST_CASE(stones_are_blocked_by_walls_and_other_stones)
{
	std::size_t width = 4;
	std::size_t height = 3;
	Array<bool> result{width, height};
	auto data = createStatus(width, height, {
			"x.*o",
			"y.o.",
			"..*o"
		});

	Status::BorderType border;
	floodFill(data.second, Point{1, 0}, result, border);
	BOOST_CHECK_EQUAL(border.size(), 1);
	CHECK_BORDER_INCLUDES(border, (Point{2, 1}));
}

BOOST_AUTO_TEST_CASE(result_is_the_same_when_using_border)
{
	std::size_t width = 4;
	std::size_t height = 3;
	Array<bool> resultBorder{width, height};
	Array<bool> resultNoBorder{width, height};
	auto data = createStatus(width, height, {
			"x.*o",
			"y.o.",
			"..*o"
		});

	Status::BorderType border;
	floodFill(data.second, Point{1, 0}, resultBorder, border);
	floodFill(data.second, Point{1, 0}, resultNoBorder);
	BOOST_CHECK_EQUAL(resultBorder, resultNoBorder);
}

BOOST_AUTO_TEST_SUITE_END() // border

BOOST_AUTO_TEST_SUITE(minmax)

BOOST_AUTO_TEST_CASE(full_table)
{
	std::size_t width = 4;
	std::size_t height = 3;
	Array<bool> result{width, height};
	auto data = createStatus(width, height, {
			"x..o",
			"yoo.",
			"...."
		});

	MinMax minmax;
	floodFill(data.second, Point{1, 0}, result, minmax);
	BOOST_CHECK_EQUAL(minmax.minX, 0);
	BOOST_CHECK_EQUAL(minmax.minY, 0);
	BOOST_CHECK_EQUAL(minmax.maxX, width - 1);
	BOOST_CHECK_EQUAL(minmax.maxY, height - 1);
}

BOOST_AUTO_TEST_CASE(one_corner)
{
	std::size_t width = 4;
	std::size_t height = 3;
	Array<bool> result{width, height};
	auto data = createStatus(width, height, {
			"x.o.",
			"yo..",
			"o..."
		});

	MinMax minmax;
	floodFill(data.second, Point{1, 0}, result, minmax);
	BOOST_CHECK_EQUAL(minmax.minX, 0);
	BOOST_CHECK_EQUAL(minmax.minY, 0);
	BOOST_CHECK_EQUAL(minmax.maxX, 1);
	BOOST_CHECK_EQUAL(minmax.maxY, 1);
}

BOOST_AUTO_TEST_CASE(enclosed_1x1_area)
{
	std::size_t width = 4;
	std::size_t height = 3;
	Array<bool> result{width, height};
	auto data = createStatus(width, height, {
			"x.o.",
			"yo.o",
			"..o."
		});

	MinMax minmax;
	floodFill(data.second, Point{2, 1}, result, minmax);
	BOOST_CHECK_EQUAL(minmax.minX, 2);
	BOOST_CHECK_EQUAL(minmax.minY, 1);
	BOOST_CHECK_EQUAL(minmax.maxX, 2);
	BOOST_CHECK_EQUAL(minmax.maxY, 1);
}

BOOST_AUTO_TEST_CASE(enclosed_bigger_area)
{
	std::size_t width = 4;
	std::size_t height = 4;
	Array<bool> result{width, height};
	auto data = createStatus(width, height, {
			"x.o.",
			"yo.o",
			"o..o",
			".oo."
		});

	MinMax minmax;
	floodFill(data.second, Point{2, 1}, result, minmax);
	BOOST_CHECK_EQUAL(minmax.minX, 1);
	BOOST_CHECK_EQUAL(minmax.minY, 1);
	BOOST_CHECK_EQUAL(minmax.maxX, 2);
	BOOST_CHECK_EQUAL(minmax.maxY, 2);
}

BOOST_AUTO_TEST_CASE(result_is_the_same_when_using_minmax)
{
	std::size_t width = 4;
	std::size_t height = 4;
	Array<bool> resultMinmax{width, height};
	Array<bool> resultNoMinmax{width, height};
	auto data = createStatus(width, height, {
			"x.o.",
			"yo.o",
			"o..o",
			".oo."
		});

	MinMax minmax;
	floodFill(data.second, Point{1, 0}, resultMinmax, minmax);
	floodFill(data.second, Point{1, 0}, resultNoMinmax);
	BOOST_CHECK_EQUAL(resultMinmax, resultNoMinmax);
}

BOOST_AUTO_TEST_SUITE_END() // minmax

BOOST_AUTO_TEST_SUITE_END() // MovableCheckerTest

