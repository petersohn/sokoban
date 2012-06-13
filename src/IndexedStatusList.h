#ifndef INDEXEDTUPLES_H_
#define INDEXEDTUPLES_H_

#include <memory>
#include <unordered_map>
#include <boost/range/iterator_range.hpp>
#include <algorithm>
#include <functional>
#include <boost/thread/locks.hpp>
#include "Mutexes.hpp"
#include "Status.h"
#include "State.h"

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
		for (const Point& p: status.state()) {
			index_.insert(std::make_pair(p, std::make_pair(val, value)));
		}
		++size_;
	}

	void clear() {
		index_.clear();
	}

	bool hasSubStatus(const Status &status, const Point &p) const
	{
		boost::shared_lock<SharedMutexType> lock(mutex_);
		RangeType range = index_.equal_range(p);
		for (const auto& val: range) {
			Status::ConstPtr sub = val.second.first;
			if (!sub->reachable(status.currentPos())) {
				continue;
			}
			bool ok = true;
			for (const Point &pp: sub->state()) {
				if (!status.state()[pp]) {
					ok = false;
					break;
				}
			}
			if (ok) {
				return true;
			}
		}
		return false;
	}

	std::vector<const Value&> getSubStatusValues(const Status &status, const Point &p) const
	{
		boost::shared_lock<SharedMutexType> lock(mutex_);
		std::vector<const Value&> result;
		RangeType range = index_.equal_range(p);
		for (const auto& val: range) {
			Status::ConstPtr sub = val.second.first;
			if (!sub->reachable(status.currentPos())) {
				continue;
			}
			bool ok = true;
			for (const Point &pp: sub->state()) {
				if (!status.state()[pp]) {
					ok = false;
					break;
				}
			}
			if (ok) {
				result.push_back(val.second.second);
			}
		}
		return std::move(result);
	}

	size_t size() const { return size_; }
};

#endif /* INDEXEDTUPLES_H_ */
