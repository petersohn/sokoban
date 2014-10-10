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

template <class Value>
class IndexedStatusList {
private:
	typedef std::unordered_multimap<Point, std::pair<Status::ConstPtr, Value>> IndexType;

	IndexType index_;
	size_t size_;

	void copyFrom(const IndexedStatusList &other)
	{
		index_.clear();
		for (const auto& elem: other.index_) {
			Status::ConstPtr copied(new Status(*(elem.second.first)));
			index_.insert(std::make_pair(elem.first, std::make_pair(copied, elem.second.second)));
		}
	}
public:
	typedef boost::iterator_range<typename IndexType::const_iterator> RangeType;

	IndexedStatusList():size_(0) {}
	IndexedStatusList(const IndexedStatusList &other)
	{
		copyFrom(other);
	}
	IndexedStatusList &operator=(const IndexedStatusList &other) {
		copyFrom(other);
		return *this;
	}

	IndexedStatusList(IndexedStatusList&&) = default;
	IndexedStatusList& operator=(IndexedStatusList&&) = default;

	void add(const Status &status, const Value& value)
	{
		Status::ConstPtr val(new Status(status));
		for (Point  p: status.state()) {
			index_.insert(std::make_pair(p, std::make_pair(val, value)));
		}
		++size_;
	}

	void clear() {
		index_.clear();
	}

	bool hasSubStatus(const Status &status, Point p) const
	{
		RangeType range = index_.equal_range(p);
		for (const auto& val: range) {
			if (isSubStatus(*val.second.first, status)) {
				return true;
			}
		}
		return false;
	}

	std::vector<Value> getSubStatusValues(const Status &status, Point p) const
	{
		std::vector<Value> result;
		RangeType range = index_.equal_range(p);
		for (const auto& val: range) {
			if (isSubStatus(*val.second.first, status)) {
				result.push_back(val.second.second);
			}
		}
		return result;
	}

	size_t size() const { return size_; }
};

#endif /* INDEXEDTUPLES_H_ */
