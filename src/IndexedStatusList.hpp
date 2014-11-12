#ifndef INDEXEDTUPLES_H_
#define INDEXEDTUPLES_H_

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
	typedef std::unordered_multimap<Point, Status> IndexType;

	IndexType index_;
	size_t size_;

public:
	typedef boost::iterator_range<typename IndexType::const_iterator> RangeType;

	IndexedStatusList():size_(0) {}
	IndexedStatusList(const IndexedStatusList&) = default;
	IndexedStatusList& operator=(const IndexedStatusList&) = default;
	IndexedStatusList(IndexedStatusList&&) = default;
	IndexedStatusList& operator=(IndexedStatusList&&) = default;

	void add(const Status& status)
	{
		for (Point  p: status.state()) {
			index_.insert({p, status});
		}
		++size_;
	}

	void clear() {
		index_.clear();
	}

	bool hasSubStatus(const Status& status, Point p) const
	{
		RangeType range = index_.equal_range(p);
		for (const auto& value: range) {
			if (isSubStatus(value.second, status)) {
				return true;
			}
		}
		return false;
	}

	size_t size() const { return size_; }
};

#endif /* INDEXEDTUPLES_H_ */
