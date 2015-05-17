#ifndef UNITTEST_CHECKERTEST_HPP
#define UNITTEST_CHECKERTEST_HPP

#include "Status/Status.hpp"

#include <boost/test/unit_test.hpp>

#define CHECK_CHECKER_RESULT(...) checkCheckerResult(__VA_ARGS__, __FILE__, __LINE__)

inline
void checkCheckerResult(const Checker& checker, const Status& status,
        bool expectedValue, const char* file, int line)
{
    BOOST_MESSAGE(">>> " << file << "(" << line << "): " <<
            boost::unit_test::framework::current_test_case().p_name <<
            ": checkCheckerResult");
    for (Point p: status.state()) {
        BOOST_MESSAGE("Checking point " << p);
        BOOST_CHECK_EQUAL(checker.check(status, p), expectedValue);
    }
    BOOST_MESSAGE("<<< " << file << "(" << line << "): " <<
            boost::unit_test::framework::current_test_case().p_name <<
            ": checkCheckerResult");
}

#endif /* UNITTEST_CHECKERTEST_HPP */
