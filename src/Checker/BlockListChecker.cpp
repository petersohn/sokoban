#include "Checker/BlockListChecker.hpp"

namespace sokoban {

bool BlockListChecker::check(const Status& status, Point  p) const
{
    return !blockList_.hasSubStatus(status, p);
}

const char* BlockListChecker::errorMessage() const
{
    return "on blocklist";
}

} // namespace sokoban
