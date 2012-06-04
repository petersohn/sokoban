#include "IndexedStatusList.h"
#include <algorithm>
#include <boost/foreach.hpp>
#include <functional>

bool IndexedStatusList::statusEqual(const Status &st1, Status::ConstPtr st2)
{
	return st2 && st1.state() == st2->state() && st2->reachable(st1.currentPos());
}

void IndexedStatusList::copyFrom(const IndexedStatusList &other)
{
	index_.clear();
	BOOST_FOREACH(IndexType::value_type elem, other.index_) {
		Status::ConstPtr copied(new Status(*(elem.second)));
		index_.insert(std::make_pair(elem.first, copied));
	}
}

void IndexedStatusList::add(const Status &status)
{
	Status::ConstPtr val(new Status(status));
	BOOST_FOREACH(const Point p, status.state()) {
		index_.insert(std::make_pair(p, val));
	}
	++size_;
}

//void IndexedStatusList::remove(const Status &status)
//{
//	size_t n = index_.size();
//	BOOST_FOREACH(const Point &p, status.state()) {
//		RangeType range = index_.equal_range(p);
//		std::remove_if(range.first, range.second, std::bind(&IndexedStatusList::statusEqual, boost::ref(status), _1));
//	}
//	if (index_.size() < n) {
//		--size_;
//	}
//}

bool IndexedStatusList::hasSubStatus(const Status &status, const Point &p)  const
{
	RangeType range = index_.equal_range(p);
	BOOST_FOREACH(IndexType::value_type val, range) {
		Status::ConstPtr sub = val.second;
		if (!sub->reachable(status.currentPos())) {
			continue;
		}
		bool ok = true;
		BOOST_FOREACH(const Point &pp, sub->state()) {
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

