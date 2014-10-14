#include "BlockListChecker.hpp"

bool BlockListChecker::check(const Status& status, Point  p)
{
	return !blockList_.hasSubStatus(status, p);
}

const char* BlockListChecker::errorMessage()
{
	return "on blocklist";
}


