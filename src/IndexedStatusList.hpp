#ifndef INDEXEDTUPLES_H_
#define INDEXEDTUPLES_H_

#include <memory>
#include <unordered_map>
#include <boost/range/iterator_range.hpp>
#include <algorithm>
#include <functional>
#include <boost/thread/locks.hpp>
#include "Mutexes.hpp"
#include "Status/Status.hpp"
#include "Status/StatusUtils.hpp"
#include "Status/State.hpp"

template <class Value>
class IndexedStatusList {
private:
	typedef std::unordered_multimap<Point, std::pair<Status::ConstPtr, Value>> IndexType;

	IndexType index_;
	size_t size_;
	mutable SharedMutexType mutex_;

	void copyFrom(const IndexedStatusList &other)
	{
		index_.clear();
		for (const auto& elem: other.index_) {
			Status::ConstPtr copied(new Status(*(elem.second)));
			index_.insert(std::make_pair(elem.first, copied));
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

	void add(const Status &status, const Value& value)
	{
		boost::unique_lock<SharedMutexType> lock(mutex_);
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
		boost::shared_lock<SharedMutexType> lock(mutex_);
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
		boost::shared_lock<SharedMutexType> lock(mutex_);
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
