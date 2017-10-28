#ifndef SRC_STATUS_ISSTATUSPOSSIBLE_HPP
#define SRC_STATUS_ISSTATUSPOSSIBLE_HPP

#include <cstdint>

namespace sokoban {

class Status;

bool isStatusPossible(const Status& status, std::size_t maxDepth);

} // namespace sokoban

#endif /* SRC_STATUS_ISSTATUSPOSSIBLE_HPP */
