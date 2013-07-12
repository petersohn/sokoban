#include "BlockListChecker.h"

bool BlockListChecker::check(const Status& status, Point  p)
{
	assert(table_ == status.tablePtr());
	return !blockList_->hasSubStatus(status, p);
}

const char* BlockListChecker::errorMessage()
{
	return "on blocklist";
}


