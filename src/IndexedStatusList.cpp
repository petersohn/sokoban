#include "IndexedStatusList.hpp"

void IndexedStatusList::add(const Status& status)
{
	if (!index_.width()) {
		index_.reset(status.width(), status.height());
	}

	for (Point  p: status.state()) {
		index_[p].push_back(status);
	}
	++size_;
}

bool IndexedStatusList::hasSubStatus(const Status& status, Point p) const
{
	if (!index_.width()) {
		return false;
	}

	for (const auto& value: index_[p]) {
		if (isSubStatus(value, status)) {
			return true;
		}
	}
	return false;
}
