#include "Status/Status.hpp"
#include "Status/StatusCreator.hpp"
#include "Status/StatusUtils.hpp"

#include <boost/test/unit_test.hpp>

using namespace sokoban;

BOOST_AUTO_TEST_SUITE(IsSubStatusTest)

BOOST_AUTO_TEST_CASE(equal)
{
    BOOST_CHECK(isSubStatus(
            createStatus({
                    "x.o.",
                    "..oy"
                }).second,
            createStatus({
                    "x.o.",
                    "..oy"
                }).second
        ));
    BOOST_CHECK(isSubStatus(
            createStatus({
                    "x.o.",
                    "..oy"
                }).second,
            createStatus({
                    "x.oy",
                    "..o."
                }).second
        ));
    BOOST_CHECK(isSubStatus(
            createStatus({
                    "x.o.",
                    "y.o."
                }).second,
            createStatus({
                    "xyo.",
                    "..o."
                }).second
        ));
}

BOOST_AUTO_TEST_CASE(equalBadReachable)
{
    BOOST_CHECK(!isSubStatus(
            createStatus({
                    "x.o.",
                    "..oy"
                }).second,
            createStatus({
                    "x.o.",
                    "y.o."
                }).second
        ));
    BOOST_CHECK(!isSubStatus(
            createStatus({
                    "x.o.",
                    "..oy"
                }).second,
            createStatus({
                    "x.o.",
                    "y.o."
                }).second
        ));
}

BOOST_AUTO_TEST_CASE(subStatus)
{
    BOOST_CHECK(isSubStatus(
            createStatus({
                    "x.o..",
                    "..oy."
                }).second,
            createStatus({
                    "x.o.o",
                    "..o.y"
                }).second
        ));
    BOOST_CHECK(isSubStatus(
            createStatus({
                    "x.o..",
                    "..oy."
                }).second,
            createStatus({
                    "x.o..",
                    "o.o.y"
                }).second
        ));
}

BOOST_AUTO_TEST_CASE(subStatusBadReachable)
{
    BOOST_CHECK(!isSubStatus(
            createStatus({
                    "x.o..",
                    "..oy."
                }).second,
            createStatus({
                    "x.o.o",
                    "y.o.."
                }).second
        ));
    BOOST_CHECK(!isSubStatus(
            createStatus({
                    "x.o..",
                    ".yo.."
                }).second,
            createStatus({
                    "x.o..",
                    "o.o.y"
                }).second
        ));
}

BOOST_AUTO_TEST_CASE(notSubStatus)
{
    BOOST_CHECK(!isSubStatus(
            createStatus({
                    "x.o.o",
                    "..oy."
                }).second,
            createStatus({
                    "x.o..",
                    "..o.y"
                }).second
        ));
    BOOST_CHECK(!isSubStatus(
            createStatus({
                    "x.o.o",
                    "..o.y"
                }).second,
            createStatus({
                    "x.oo.",
                    "..o.y"
                }).second
        ));
}


BOOST_AUTO_TEST_SUITE_END() // IsSubStatus

