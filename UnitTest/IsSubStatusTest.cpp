#include "Status/Status.hpp"
#include "Status/StatusUtils.hpp"
#include "Status/StatusCreator.hpp"
#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_SUITE(IsSubStatus)

BOOST_AUTO_TEST_CASE(equal)
{
	BOOST_CHECK(isSubStatus(
			createStatus(4, 2, {
					"x.o.",
					"..oy"
				}).second,
			createStatus(4, 2, {
					"x.o.",
					"..oy"
				}).second
		));
	BOOST_CHECK(isSubStatus(
			createStatus(4, 2, {
					"x.o.",
					"..oy"
				}).second,
			createStatus(4, 2, {
					"x.oy",
					"..o."
				}).second
		));
	BOOST_CHECK(isSubStatus(
			createStatus(4, 2, {
					"x.o.",
					"y.o."
				}).second,
			createStatus(4, 2, {
					"xyo.",
					"..o."
				}).second
		));
}

BOOST_AUTO_TEST_CASE(equalBadReachable)
{
	BOOST_CHECK(!isSubStatus(
			createStatus(4, 2, {
					"x.o.",
					"..oy"
				}).second,
			createStatus(4, 2, {
					"x.o.",
					"y.o."
				}).second
		));
	BOOST_CHECK(!isSubStatus(
			createStatus(4, 2, {
					"x.o.",
					"..oy"
				}).second,
			createStatus(4, 2, {
					"x.o.",
					"y.o."
				}).second
		));
}

BOOST_AUTO_TEST_CASE(subStatus)
{
	BOOST_CHECK(isSubStatus(
			createStatus(5, 2, {
					"x.o..",
					"..oy."
				}).second,
			createStatus(5, 2, {
					"x.o.o",
					"..o.y"
				}).second
		));
	BOOST_CHECK(isSubStatus(
			createStatus(5, 2, {
					"x.o..",
					"..oy."
				}).second,
			createStatus(5, 2, {
					"x.o..",
					"o.o.y"
				}).second
		));
}

BOOST_AUTO_TEST_CASE(subStatusBadReachable)
{
	BOOST_CHECK(!isSubStatus(
			createStatus(5, 2, {
					"x.o..",
					"..oy."
				}).second,
			createStatus(5, 2, {
					"x.o.o",
					"y.o.."
				}).second
		));
	BOOST_CHECK(!isSubStatus(
			createStatus(5, 2, {
					"x.o..",
					".yo.."
				}).second,
			createStatus(5, 2, {
					"x.o..",
					"o.o.y"
				}).second
		));
}

BOOST_AUTO_TEST_CASE(notSubStatus)
{
	BOOST_CHECK(!isSubStatus(
			createStatus(5, 2, {
					"x.o.o",
					"..oy."
				}).second,
			createStatus(5, 2, {
					"x.o..",
					"..o.y"
				}).second
		));
	BOOST_CHECK(!isSubStatus(
			createStatus(5, 2, {
					"x.o.o",
					"..o.y"
				}).second,
			createStatus(5, 2, {
					"x.oo.",
					"..o.y"
				}).second
		));
}


BOOST_AUTO_TEST_SUITE_END() // IsSubStatus

