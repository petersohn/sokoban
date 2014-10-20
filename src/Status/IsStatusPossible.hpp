#ifndef SRC_STATUS_ISSTATUSPOSSIBLE_HPP
#define SRC_STATUS_ISSTATUSPOSSIBLE_HPP

#include <cstdint>

class Status;

bool isStatusPossible(const Status& status, std::size_t maxDepth);


#endif /* SRC_STATUS_ISSTATUSPOSSIBLE_HPP */
