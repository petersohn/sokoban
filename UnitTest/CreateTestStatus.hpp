#ifndef UNITTEST_CREATETESTSTATUS_HPP
#define UNITTEST_CREATETESTSTATUS_HPP

#include "Status/Status.hpp"

#include <string>

namespace sokoban {

Status createTestStatus(const Table& table, const std::vector<std::string>& lines);

} // namespace sokoban

#endif /* UNITTEST_CREATETESTSTATUS_HPP */
