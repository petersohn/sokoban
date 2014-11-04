#ifndef UNITTEST_CREATETESTSTATUS_HPP
#define UNITTEST_CREATETESTSTATUS_HPP

#include "Status/Status.hpp"
#include <string>

Status createTestStatus(const Table& table, const std::vector<std::string>& lines);

#endif /* UNITTEST_CREATETESTSTATUS_HPP */
