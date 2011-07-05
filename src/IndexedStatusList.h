#ifndef INDEXEDTUPLES_H_
#define INDEXEDTUPLES_H_

#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>
#include "Status.h"
#include "State.h"

class IndexedStatusList {
private:
	typedef boost::unordered_multimap<Point, Status::ConstPtr> IndexType;

	IndexType index_;
	size_t size_;

	void copyFrom(const IndexedStatusList &other);

	static bool statusEqual(const Status &st1, Status::ConstPtr st2);
public:
	typedef std::pair<IndexType::const_iterator, IndexType::const_iterator> RangeType;

	IndexedStatusList():size_(0) {}
	IndexedStatusList(const IndexedStatusList &other)
	{
		copyFrom(other);
	}
	IndexedStatusList &operator=(const IndexedStatusList &other) {
		copyFrom(other);
		return *this;
	}

	void add(const Status &status);
//	void remove(const Status &status);
	void clear() {
		index_.clear();
	}
	bool hasSubStatus(const Status &status, const Point &p) const;
	size_t size() const { return size_; }
};

#endif /* INDEXEDTUPLES_H_ */
