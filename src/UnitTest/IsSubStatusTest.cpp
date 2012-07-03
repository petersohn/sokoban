#include "Status.h"
#include "StatusCreator.h"
#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_SUITE(IsSubStatus)

BOOST_AUTO_TEST_CASE(equal)
{
	BOOST_CHECK(isSubStatus(
			createStatus(4, 2, {
					"x.o.",
					"..oy"
				}),
			createStatus(4, 2, {
					"x.o.",
					"..oy"
				})
		));
	BOOST_CHECK(isSubStatus(
			createStatus(4, 2, {
					"x.o.",
					"..oy"
				}),
			createStatus(4, 2, {
					"x.oy",
					"..o."
				})
		));
	BOOST_CHECK(isSubStatus(
			createStatus(4, 2, {
					"x.o.",
					"y.o."
				}),
			createStatus(4, 2, {
					"xyo.",
					"..o."
				})
		));
}

BOOST_AUTO_TEST_CASE(equalBadReachable)
{
	BOOST_CHECK(!isSubStatus(
			createStatus(4, 2, {
					"x.o.",
					"..oy"
				}),
			createStatus(4, 2, {
					"x.o.",
					"y.o."
				})
		));
	BOOST_CHECK(!isSubStatus(
			createStatus(4, 2, {
					"x.o.",
					"..oy"
				}),
			createStatus(4, 2, {
					"x.o.",
					"y.o."
				})
		));
}




BOOST_AUTO_TEST_SUITE_END() // IsSubStatus

