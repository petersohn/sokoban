#include <boost/test/unit_test.hpp>
#include <unordered_set>
#include "Status/PointRange.hpp"


BOOST_AUTO_TEST_SUITE(PointRangeTest)

BOOST_AUTO_TEST_CASE(Endpoints)
{
	Point begin(1,0);
	Point end(3,4);
	PointRange range(begin, end);
	BOOST_CHECK_EQUAL(range.front(), begin);
	BOOST_CHECK_EQUAL(range.back(), end + Point::pm1m1);
}

BOOST_AUTO_TEST_CASE(EmptyRange)
{
	Point begin(2,1);
	Point end(2,1);
	PointRange range(begin, end);
	BOOST_CHECK(range.begin() == range.end());
	BOOST_CHECK_THROW(range.front(), std::out_of_range);
	BOOST_CHECK_THROW(range.back(), std::out_of_range);
}


BOOST_AUTO_TEST_CASE(SinglePointRange)
{
	Point p(1,10);
	PointRange range(p, p+Point::p11);
	BOOST_CHECK_EQUAL(range.front(), p);
	BOOST_CHECK_EQUAL(range.back(), p);
	PointRange::iterator it = range.begin();
	BOOST_CHECK_EQUAL(*it, p);
	BOOST_CHECK(++it == range.end());
}

BOOST_AUTO_TEST_SUITE(InRange)

BOOST_AUTO_TEST_CASE(ForwardForward)
{
	Point begin(2,2);
	Point end(5,7);
	PointRange range(begin, end);
	std::unordered_set<Point> pointsConsidered;
	for (Point  p: range) {
		BOOST_MESSAGE(p);
		BOOST_REQUIRE_GE(p.x, begin.x);
		BOOST_REQUIRE_LT(p.x, end.x);
		BOOST_REQUIRE_GE(p.y, begin.y);
		BOOST_REQUIRE_LT(p.y, end.y);
		BOOST_REQUIRE_EQUAL(pointsConsidered.count(p), 0);
		pointsConsidered.insert(p);
	}
}

BOOST_AUTO_TEST_CASE(Forwardendward)
{
	Point begin(1,8);
	Point end(10,3);
	PointRange range(begin, end);
	std::unordered_set<Point> pointsConsidered;
	for (Point  p: range) {
		BOOST_MESSAGE(p);
		BOOST_REQUIRE_GE(p.x, begin.x);
		BOOST_REQUIRE_LT(p.x, end.x);
		BOOST_REQUIRE_LE(p.y, begin.y);
		BOOST_REQUIRE_GT(p.y, end.y);
		BOOST_REQUIRE_EQUAL(pointsConsidered.count(p), 0);
		pointsConsidered.insert(p);
	}
}

BOOST_AUTO_TEST_CASE(endwardForward)
{
	Point begin(9,-1);
	Point end(-3,5);
	PointRange range(begin, end);
	std::unordered_set<Point> pointsConsidered;
	for (Point  p: range) {
		BOOST_REQUIRE_LE(p.x, begin.x);
		BOOST_REQUIRE_GT(p.x, end.x);
		BOOST_REQUIRE_GE(p.y, begin.y);
		BOOST_REQUIRE_LT(p.y, end.y);
		BOOST_REQUIRE_EQUAL(pointsConsidered.count(p), 0);
		pointsConsidered.insert(p);
	}
}

BOOST_AUTO_TEST_CASE(endwardendward)
{
	Point begin(15,-2);
	Point end(10,-20);
	PointRange range(begin, end);
	std::unordered_set<Point> pointsConsidered;
	for (Point  p: range) {
		BOOST_MESSAGE(p);
		BOOST_REQUIRE_LE(p.x, begin.x);
		BOOST_REQUIRE_GT(p.x, end.x);
		BOOST_REQUIRE_LE(p.y, begin.y);
		BOOST_REQUIRE_GT(p.y, end.y);
		BOOST_REQUIRE_EQUAL(pointsConsidered.count(p), 0);
		pointsConsidered.insert(p);
	}
}

BOOST_AUTO_TEST_SUITE_END() // InRange

BOOST_AUTO_TEST_SUITE(Iterators)

BOOST_AUTO_TEST_SUITE(PrePlus)

BOOST_AUTO_TEST_CASE(ForwardForward)
{
	Point begin(2,2);
	Point end(4,5);
	PointRange range(begin, end);
	PointRange::iterator it = range.begin();
	BOOST_CHECK_EQUAL(*it, Point(2,2));
	BOOST_CHECK_EQUAL(*++it, Point(3,2));
	BOOST_CHECK_EQUAL(*++it, Point(2,3));
	BOOST_CHECK_EQUAL(*++it, Point(3,3));
	BOOST_CHECK_EQUAL(*++it, Point(2,4));
	BOOST_CHECK_EQUAL(*++it, Point(3,4));
	BOOST_CHECK(++it == range.end());
}

BOOST_AUTO_TEST_CASE(Forwardendward)
{
	Point begin(0,0);
	Point end(2,-2);
	PointRange range(begin, end);
	PointRange::iterator it = range.begin();
	BOOST_CHECK_EQUAL(*it, Point(0,0));
	BOOST_CHECK_EQUAL(*++it, Point(1,0));
	BOOST_CHECK_EQUAL(*++it, Point(0,-1));
	BOOST_CHECK_EQUAL(*++it, Point(1,-1));
	BOOST_CHECK(++it == range.end());
}

BOOST_AUTO_TEST_CASE(endwardForward)
{
	Point begin(1,-1);
	Point end(-2,2);
	PointRange range(begin, end);
	PointRange::iterator it = range.begin();
	BOOST_CHECK_EQUAL(*it, Point(1,-1));
	BOOST_CHECK_EQUAL(*++it, Point(0,-1));
	BOOST_CHECK_EQUAL(*++it, Point(-1,-1));
	BOOST_CHECK_EQUAL(*++it, Point(1,0));
	BOOST_CHECK_EQUAL(*++it, Point(0,0));
	BOOST_CHECK_EQUAL(*++it, Point(-1,0));
	BOOST_CHECK_EQUAL(*++it, Point(1,1));
	BOOST_CHECK_EQUAL(*++it, Point(0,1));
	BOOST_CHECK_EQUAL(*++it, Point(-1,1));
	BOOST_CHECK(++it == range.end());
}

BOOST_AUTO_TEST_CASE(endwardendward)
{
	Point begin(10,5);
	Point end(7,3);
	PointRange range(begin, end);
	PointRange::iterator it = range.begin();
	BOOST_CHECK_EQUAL(*it, Point(10,5));
	BOOST_CHECK_EQUAL(*++it, Point(9,5));
	BOOST_CHECK_EQUAL(*++it, Point(8,5));
	BOOST_CHECK_EQUAL(*++it, Point(10,4));
	BOOST_CHECK_EQUAL(*++it, Point(9,4));
	BOOST_CHECK_EQUAL(*++it, Point(8,4));
	BOOST_CHECK(++it == range.end());
}

BOOST_AUTO_TEST_SUITE_END() // PrePlus

BOOST_AUTO_TEST_SUITE(PreMinus)

BOOST_AUTO_TEST_CASE(ForwardForward)
{
	Point begin(2,2);
	Point end(4,5);
	PointRange range(begin, end);
	PointRange::iterator it = range.end();
	BOOST_CHECK(it == range.end());
	BOOST_CHECK_EQUAL(*--it, Point(3,4));
	BOOST_CHECK_EQUAL(*--it, Point(2,4));
	BOOST_CHECK_EQUAL(*--it, Point(3,3));
	BOOST_CHECK_EQUAL(*--it, Point(2,3));
	BOOST_CHECK_EQUAL(*--it, Point(3,2));
	BOOST_CHECK_EQUAL(*--it, Point(2,2));
}

BOOST_AUTO_TEST_CASE(Forwardendward)
{
	Point begin(0,0);
	Point end(2,-2);
	PointRange range(begin, end);
	PointRange::iterator it = range.end();
	BOOST_CHECK(it == range.end());
	BOOST_CHECK_EQUAL(*--it, Point(1,-1));
	BOOST_CHECK_EQUAL(*--it, Point(0,-1));
	BOOST_CHECK_EQUAL(*--it, Point(1,0));
	BOOST_CHECK_EQUAL(*--it, Point(0,0));
}

BOOST_AUTO_TEST_CASE(endwardForward)
{
	Point begin(1,-1);
	Point end(-2,2);
	PointRange range(begin, end);
	PointRange::iterator it = range.end();
	BOOST_CHECK(it == range.end());
	BOOST_CHECK_EQUAL(*--it, Point(-1,1));
	BOOST_CHECK_EQUAL(*--it, Point(0,1));
	BOOST_CHECK_EQUAL(*--it, Point(1,1));
	BOOST_CHECK_EQUAL(*--it, Point(-1,0));
	BOOST_CHECK_EQUAL(*--it, Point(0,0));
	BOOST_CHECK_EQUAL(*--it, Point(1,0));
	BOOST_CHECK_EQUAL(*--it, Point(-1,-1));
	BOOST_CHECK_EQUAL(*--it, Point(0,-1));
	BOOST_CHECK_EQUAL(*--it, Point(1,-1));
}

BOOST_AUTO_TEST_CASE(endwardendward)
{
	Point begin(10,5);
	Point end(7,3);
	PointRange range(begin, end);
	PointRange::iterator it = range.end();
	BOOST_CHECK(it == range.end());
	BOOST_CHECK_EQUAL(*--it, Point(8,4));
	BOOST_CHECK_EQUAL(*--it, Point(9,4));
	BOOST_CHECK_EQUAL(*--it, Point(10,4));
	BOOST_CHECK_EQUAL(*--it, Point(8,5));
	BOOST_CHECK_EQUAL(*--it, Point(9,5));
	BOOST_CHECK_EQUAL(*--it, Point(10,5));
}

BOOST_AUTO_TEST_SUITE_END() // PreMinus

BOOST_AUTO_TEST_SUITE(PostPlus)

BOOST_AUTO_TEST_CASE(ForwardForward)
{
	Point begin(2,2);
	Point end(4,5);
	PointRange range(begin, end);
	PointRange::iterator it = range.begin();
	BOOST_CHECK_EQUAL(*it++, Point(2,2));
	BOOST_CHECK_EQUAL(*it++, Point(3,2));
	BOOST_CHECK_EQUAL(*it++, Point(2,3));
	BOOST_CHECK_EQUAL(*it++, Point(3,3));
	BOOST_CHECK_EQUAL(*it++, Point(2,4));
	BOOST_CHECK_EQUAL(*it++, Point(3,4));
	BOOST_CHECK(it == range.end());
}

BOOST_AUTO_TEST_CASE(Forwardendward)
{
	Point begin(0,0);
	Point end(2,-2);
	PointRange range(begin, end);
	PointRange::iterator it = range.begin();
	BOOST_CHECK_EQUAL(*it++, Point(0,0));
	BOOST_CHECK_EQUAL(*it++, Point(1,0));
	BOOST_CHECK_EQUAL(*it++, Point(0,-1));
	BOOST_CHECK_EQUAL(*it++, Point(1,-1));
	BOOST_CHECK(it == range.end());
}

BOOST_AUTO_TEST_CASE(endwardForward)
{
	Point begin(1,-1);
	Point end(-2,2);
	PointRange range(begin, end);
	PointRange::iterator it = range.begin();
	BOOST_CHECK_EQUAL(*it++, Point(1,-1));
	BOOST_CHECK_EQUAL(*it++, Point(0,-1));
	BOOST_CHECK_EQUAL(*it++, Point(-1,-1));
	BOOST_CHECK_EQUAL(*it++, Point(1,0));
	BOOST_CHECK_EQUAL(*it++, Point(0,0));
	BOOST_CHECK_EQUAL(*it++, Point(-1,0));
	BOOST_CHECK_EQUAL(*it++, Point(1,1));
	BOOST_CHECK_EQUAL(*it++, Point(0,1));
	BOOST_CHECK_EQUAL(*it++, Point(-1,1));
	BOOST_CHECK(it == range.end());
}

BOOST_AUTO_TEST_CASE(endwardendward)
{
	Point begin(10,5);
	Point end(7,3);
	PointRange range(begin, end);
	PointRange::iterator it = range.begin();
	BOOST_CHECK_EQUAL(*it++, Point(10,5));
	BOOST_CHECK_EQUAL(*it++, Point(9,5));
	BOOST_CHECK_EQUAL(*it++, Point(8,5));
	BOOST_CHECK_EQUAL(*it++, Point(10,4));
	BOOST_CHECK_EQUAL(*it++, Point(9,4));
	BOOST_CHECK_EQUAL(*it++, Point(8,4));
	BOOST_CHECK(it == range.end());
}

BOOST_AUTO_TEST_SUITE_END() // PostPlus

BOOST_AUTO_TEST_SUITE(PostMinus)

BOOST_AUTO_TEST_CASE(ForwardForward)
{
	Point begin(2,2);
	Point end(4,5);
	PointRange range(begin, end);
	PointRange::iterator it = range.end();
	BOOST_CHECK(it-- == range.end());
	BOOST_CHECK_EQUAL(*it--, Point(3,4));
	BOOST_CHECK_EQUAL(*it--, Point(2,4));
	BOOST_CHECK_EQUAL(*it--, Point(3,3));
	BOOST_CHECK_EQUAL(*it--, Point(2,3));
	BOOST_CHECK_EQUAL(*it--, Point(3,2));
	BOOST_CHECK_EQUAL(*it, Point(2,2));
}

BOOST_AUTO_TEST_CASE(Forwardendward)
{
	Point begin(0,0);
	Point end(2,-2);
	PointRange range(begin, end);
	PointRange::iterator it = range.end();
	BOOST_CHECK(it-- == range.end());
	BOOST_CHECK_EQUAL(*it--, Point(1,-1));
	BOOST_CHECK_EQUAL(*it--, Point(0,-1));
	BOOST_CHECK_EQUAL(*it--, Point(1,0));
	BOOST_CHECK_EQUAL(*it, Point(0,0));
}

BOOST_AUTO_TEST_CASE(endwardForward)
{
	Point begin(1,-1);
	Point end(-2,2);
	PointRange range(begin, end);
	PointRange::iterator it = range.end();
	BOOST_CHECK(it-- == range.end());
	BOOST_CHECK_EQUAL(*it--, Point(-1,1));
	BOOST_CHECK_EQUAL(*it--, Point(0,1));
	BOOST_CHECK_EQUAL(*it--, Point(1,1));
	BOOST_CHECK_EQUAL(*it--, Point(-1,0));
	BOOST_CHECK_EQUAL(*it--, Point(0,0));
	BOOST_CHECK_EQUAL(*it--, Point(1,0));
	BOOST_CHECK_EQUAL(*it--, Point(-1,-1));
	BOOST_CHECK_EQUAL(*it--, Point(0,-1));
	BOOST_CHECK_EQUAL(*it, Point(1,-1));
}

BOOST_AUTO_TEST_CASE(endwardendward)
{
	Point begin(10,5);
	Point end(7,3);
	PointRange range(begin, end);
	PointRange::iterator it = range.end();
	BOOST_CHECK(it-- == range.end());
	BOOST_CHECK_EQUAL(*it--, Point(8,4));
	BOOST_CHECK_EQUAL(*it--, Point(9,4));
	BOOST_CHECK_EQUAL(*it--, Point(10,4));
	BOOST_CHECK_EQUAL(*it--, Point(8,5));
	BOOST_CHECK_EQUAL(*it--, Point(9,5));
	BOOST_CHECK_EQUAL(*it, Point(10,5));
}

BOOST_AUTO_TEST_SUITE_END() // PostMinus

BOOST_AUTO_TEST_SUITE_END() // Iterators

BOOST_AUTO_TEST_SUITE_END() // PointRangeTest

