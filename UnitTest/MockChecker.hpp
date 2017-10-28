#ifndef UNITTEST_MOCKCHEKCER_HPP
#define UNITTEST_MOCKCHEKCER_HPP

#include "Checker/Checker.hpp"
#include "Status/Point.hpp"

#include <turtle/mock.hpp>

MOCK_BASE_CLASS(MockChecker, sokoban::Checker) {
    MOCK_CONST_METHOD(check, 2, bool(const sokoban::Status&, sokoban::Point))
    MOCK_CONST_METHOD(errorMessage, 0, const char*())
};


#endif /* UNITTEST_MOCKCHEKCER_HPP */

