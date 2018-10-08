#include "Status/floodFill.hpp"
#include "Status/StatusCreator.hpp"

#include "MatrixIO.hpp"
#include "BorderTypeIO.hpp"

#include <boost/test/unit_test.hpp>

#include <algorithm>

using namespace sokoban;

BOOST_AUTO_TEST_SUITE(MovableCheckerTest)

BOOST_AUTO_TEST_SUITE(reachability)

BOOST_AUTO_TEST_CASE(floors_are_reachable_but_walls_and_stones_are_not)
{
    std::size_t width = 4;
    std::size_t height = 2;
    Matrix<bool> result{width, height};
    auto data = createStatus({
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
    Matrix<bool> result{width, height};
    auto data = createStatus({
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
    Matrix<bool> result{width, height};
    auto data = createStatus({
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
    Matrix<bool> result{width, height};
    auto data = createStatus({
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

BOOST_AUTO_TEST_CASE(single_stone_in_the_middle)
{
    std::size_t width = 4;
    std::size_t height = 3;
    Matrix<bool> result{width, height};
    auto data = createStatus({
            "x...",
            "y.o."
            "...."
        });

    Status::BorderType border;
    floodFill(data.second, Point{1, 0}, result, border);
    BOOST_CHECK_EQUAL(border, (Status::BorderType{Point{2, 1}}));

}

BOOST_AUTO_TEST_CASE(multiple_reachable_stones)
{
    std::size_t width = 4;
    std::size_t height = 3;
    Matrix<bool> result{width, height};
    auto data = createStatus({
            "x...",
            "y.o.",
            ".o.o"
        });

    Status::BorderType border;
    floodFill(data.second, Point{1, 0}, result, border);
    std::sort(border.begin(), border.end());
    BOOST_CHECK_EQUAL(border, (Status::BorderType{
                Point{2, 1}, Point{1, 2}, Point{3, 2}}));
}

BOOST_AUTO_TEST_CASE(one_stone_is_blocked_by_other_stones)
{
    std::size_t width = 4;
    std::size_t height = 3;
    Matrix<bool> result{width, height};
    auto data = createStatus({
            "x..o",
            "y.o.",
            ".o.o"
        });

    Status::BorderType border;
    floodFill(data.second, Point{1, 0}, result, border);
    std::sort(border.begin(), border.end());
    BOOST_CHECK_EQUAL(border, (Status::BorderType{
                Point{3, 0}, Point{2, 1}, Point{1, 2}}));
}

BOOST_AUTO_TEST_CASE(stones_are_blocked_by_walls_and_other_stones)
{
    std::size_t width = 4;
    std::size_t height = 3;
    Matrix<bool> result{width, height};
    auto data = createStatus({
            "x.*o",
            "y.o.",
            "..*o"
        });

    Status::BorderType border;
    floodFill(data.second, Point{1, 0}, result, border);
    BOOST_CHECK_EQUAL(border.size(), 1);
    BOOST_CHECK_EQUAL(border, (Status::BorderType{Point{2, 1}}));
}

BOOST_AUTO_TEST_CASE(result_is_the_same_when_using_border)
{
    std::size_t width = 4;
    std::size_t height = 3;
    Matrix<bool> resultBorder{width, height};
    Matrix<bool> resultNoBorder{width, height};
    auto data = createStatus({
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
    Matrix<bool> result{width, height};
    auto data = createStatus({
            "x..o",
            "yoo.",
            "...."
        });

    MinMax minmax;
    floodFill(data.second, Point{1, 0}, result, minmax);
    BOOST_CHECK_EQUAL(minmax, (MinMax{0, 3, 0, 2}));
}

BOOST_AUTO_TEST_CASE(one_corner)
{
    std::size_t width = 4;
    std::size_t height = 3;
    Matrix<bool> result{width, height};
    auto data = createStatus({
            "x.o.",
            "yo..",
            "o..."
        });

    MinMax minmax;
    floodFill(data.second, Point{1, 0}, result, minmax);
    BOOST_CHECK_EQUAL(minmax, (MinMax{0, 1, 0, 1}));
}

BOOST_AUTO_TEST_CASE(enclosed_1x1_area)
{
    std::size_t width = 4;
    std::size_t height = 3;
    Matrix<bool> result{width, height};
    auto data = createStatus({
            "x.o.",
            "yo.o",
            "..o."
        });

    MinMax minmax;
    floodFill(data.second, Point{2, 1}, result, minmax);
    BOOST_CHECK_EQUAL(minmax, (MinMax{2, 2, 1, 1}));
}

BOOST_AUTO_TEST_CASE(enclosed_bigger_area)
{
    std::size_t width = 4;
    std::size_t height = 4;
    Matrix<bool> result{width, height};
    auto data = createStatus({
            "x.o.",
            "yo.o",
            "o..o",
            ".oo."
        });

    MinMax minmax;
    floodFill(data.second, Point{2, 1}, result, minmax);
    BOOST_CHECK_EQUAL(minmax, (MinMax{1, 2, 1, 2}));
}

BOOST_AUTO_TEST_CASE(result_is_the_same_when_using_minmax)
{
    std::size_t width = 4;
    std::size_t height = 4;
    Matrix<bool> resultMinmax{width, height};
    Matrix<bool> resultNoMinmax{width, height};
    auto data = createStatus({
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

BOOST_AUTO_TEST_SUITE(border_minmax)

BOOST_AUTO_TEST_CASE(result_is_the_same_when_using_minmax)
{
    std::size_t width = 4;
    std::size_t height = 4;
    Matrix<bool> resultMinmax{width, height};
    Matrix<bool> resultBorder{width, height};
    Matrix<bool> result{width, height};
    auto data = createStatus({
            "x.o.",
            "yo.o",
            "o..o",
            ".oo."
        });

    MinMax minmaxNoBorder;
    MinMax minmaxWithBorder;
    Status::BorderType borderNoMinmax;
    Status::BorderType borderWithMinmax;
    floodFill(data.second, Point{1, 0}, result, minmaxWithBorder, borderWithMinmax);
    floodFill(data.second, Point{1, 0}, resultMinmax, minmaxNoBorder);
    floodFill(data.second, Point{1, 0}, resultBorder, borderNoMinmax);

    BOOST_CHECK_EQUAL(resultMinmax, result);
    BOOST_CHECK_EQUAL(resultBorder, result);
    BOOST_CHECK_EQUAL(minmaxWithBorder, minmaxNoBorder);
    BOOST_CHECK_EQUAL(borderWithMinmax, borderNoMinmax);
}

BOOST_AUTO_TEST_SUITE_END() // border_minmax

BOOST_AUTO_TEST_SUITE_END() // MovableCheckerTest

