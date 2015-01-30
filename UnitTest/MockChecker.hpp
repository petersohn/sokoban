#ifndef UNITTEST_MOCKCHEKCER_HPP
#define UNITTEST_MOCKCHEKCER_HPP

#include "Checker.hpp"
#include "Status/Point.hpp"
#include <turtle/mock.hpp>

MOCK_BASE_CLASS(MockChecker, Checker) {
	MOCK_CONST_METHOD(check, 2, bool(const Status&, Point))
	MOCK_CONST_METHOD(errorMessage, 0, const char*())
};


#endif /* UNITTEST_MOCKCHEKCER_HPP */

