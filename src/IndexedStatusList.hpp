#ifndef SRC_INDEXEDSTATUSLIST_HPP
#define SRC_INDEXEDSTATUSLIST_HPP

#include <memory>
#include <unordered_map>
#include <boost/range/iterator_range.hpp>
#include <algorithm>
#include <functional>
#include <boost/thread/locks.hpp>
#include "Status/Status.hpp"
#include "Status/StatusUtils.hpp"
#include "Status/State.hpp"

class IndexedStatusList {
private:
	Array<std::vector<Status>> index_;
	size_t size_;

public:
	IndexedStatusList():size_(0) {}
	IndexedStatusList(const IndexedStatusList&) = default;
	IndexedStatusList& operator=(const IndexedStatusList&) = default;
	IndexedStatusList(IndexedStatusList&&) = default;
	IndexedStatusList& operator=(IndexedStatusList&&) = default;

	void add(const Status& status)
	{
		if (!index_.width()) {
			index_.reset(status.width(), status.height());
		}

		for (Point  p: status.state()) {
			index_[p].push_back(status);
		}
		++size_;
	}

	void clear() {
		index_.reset(0, 0);
		size_ = 0;
	}

	bool hasSubStatus(const Status& status, Point p) const
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

	size_t size() const { return size_; }
};

#endif /* SRC_INDEXEDSTATUSLIST_HPP */
