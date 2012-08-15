#include <boost/test/unit_test.hpp>
#include <unordered_set>
#include "Status/PointRange.h"


BOOST_AUTO_TEST_SUITE(PointRangeTest)

BOOST_AUTO_TEST_CASE(Endpoints)
{
	Point front(1,0);
	Point back(3,4);
	PointRange range(front, back);
	BOOST_CHECK_EQUAL(range.front(), front);
	BOOST_CHECK_EQUAL(range.back(), back);
}

BOOST_AUTO_TEST_SUITE(InRange)

BOOST_AUTO_TEST_CASE(ForwardForward)
{
	Point front(2,2);
	Point back(5,7);
	PointRange range(front, back);
	std::unordered_set<Point> pointsConsidered;
	for (const Point& p: range) {
		BOOST_REQUIRE_GE(p.x, front.x);
		BOOST_REQUIRE_LE(p.x, back.x);
		BOOST_REQUIRE_GE(p.y, front.y);
		BOOST_REQUIRE_LE(p.y, back.y);
		BOOST_REQUIRE_EQUAL(pointsConsidered.count(p), 0);
		pointsConsidered.insert(p);
	}
}

BOOST_AUTO_TEST_CASE(ForwardBackward)
{
	Point front(1,8);
	Point back(10,3);
	PointRange range(front, back);
	std::unordered_set<Point> pointsConsidered;
	for (const Point& p: range) {
		BOOST_REQUIRE_GE(p.x, front.x);
		BOOST_REQUIRE_LE(p.x, back.x);
		BOOST_REQUIRE_LE(p.y, front.y);
		BOOST_REQUIRE_GE(p.y, back.y);
		BOOST_REQUIRE_EQUAL(pointsConsidered.count(p), 0);
		pointsConsidered.insert(p);
	}
}

BOOST_AUTO_TEST_CASE(BackwardForward)
{
	Point front(9,-1);
	Point back(-3,5);
	PointRange range(front, back);
	std::unordered_set<Point> pointsConsidered;
	for (const Point& p: range) {
		BOOST_REQUIRE_LE(p.x, front.x);
		BOOST_REQUIRE_GE(p.x, back.x);
		BOOST_REQUIRE_GE(p.y, front.y);
		BOOST_REQUIRE_LE(p.y, back.y);
		BOOST_REQUIRE_EQUAL(pointsConsidered.count(p), 0);
		pointsConsidered.insert(p);
	}
}

BOOST_AUTO_TEST_CASE(BackwardBackward)
{
	Point front(15,-2);
	Point back(10,-20);
	PointRange range(front, back);
	std::unordered_set<Point> pointsConsidered;
	for (const Point& p: range) {
		BOOST_REQUIRE_LE(p.x, front.x);
		BOOST_REQUIRE_GE(p.x, back.x);
		BOOST_REQUIRE_LE(p.y, front.y);
		BOOST_REQUIRE_GE(p.y, back.y);
		BOOST_REQUIRE_EQUAL(pointsConsidered.count(p), 0);
		pointsConsidered.insert(p);
	}
}

BOOST_AUTO_TEST_SUITE_END() // InRange

BOOST_AUTO_TEST_SUITE(Iterators)

BOOST_AUTO_TEST_SUITE(PrePlus)

BOOST_AUTO_TEST_CASE(ForwardForward)
{
	Point front(2,2);
	Point back(3,4);
	PointRange range(front, back);
	PointRange::iterator it = range.begin();
	BOOST_CHECK_EQUAL(*it, Point(2,2));
	BOOST_CHECK_EQUAL(*++it, Point(3,2));
	BOOST_CHECK_EQUAL(*++it, Point(2,3));
	BOOST_CHECK_EQUAL(*++it, Point(3,3));
	BOOST_CHECK_EQUAL(*++it, Point(2,4));
	BOOST_CHECK_EQUAL(*++it, Point(3,4));
	BOOST_CHECK(++it == range.end());
}

BOOST_AUTO_TEST_SUITE_END() // PrePlus

BOOST_AUTO_TEST_SUITE_END() // Iterators

BOOST_AUTO_TEST_SUITE_END() // PointRangeTest

