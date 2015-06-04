#ifndef UNITTEST_OPERATORCHECKS_HPP
#define UNITTEST_OPERATORCHECKS_HPP

#include <boost/test/unit_test.hpp>

#define CHECK_EQ(lhs, rhs) do { \
    BOOST_CHECK((lhs) == (rhs)); \
    BOOST_CHECK((rhs) == (lhs)); \
    BOOST_CHECK(!((lhs) != (rhs))); \
    BOOST_CHECK(!((rhs) != (lhs))); \
} while (false)

#define CHECK_NE(lhs, rhs) do { \
    BOOST_CHECK((lhs) != (rhs)); \
    BOOST_CHECK((rhs) != (lhs)); \
    BOOST_CHECK(!((lhs) == (rhs))); \
    BOOST_CHECK(!((rhs) == (lhs))); \
} while (false)



#endif // UNITTEST_OPERATORCHECKS_HPP
