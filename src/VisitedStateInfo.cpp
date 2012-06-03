#include "VisitedStateInfo.h"
#include "Status.h"
#include <boost/functional/hash.hpp>

VisitedStateInfo::VisitedStateInfo(const Status& status):
	state_(status.state())
{
	Point p;
	for (p.x = 0; p.x < status.width(); ++p.x) {
		for (p.y = 0; p.y < status.height(); ++p.y) {
			if (status.reachable(p)) {
				firstReachable_ = p;
				return;
			}
		}
	}
	assert(false && "Impossible status");
}


bool operator==(const VisitedStateInfo& left, const VisitedStateInfo& right)
{
	return left.firstReachable() == right.firstReachable() && left.state() == right.state();
}

size_t hash_value(const VisitedStateInfo& state)
{
	size_t seed = 0;
	boost::hash_combine(seed, state.state());
	boost::hash_combine(seed, state.firstReachable());
	return seed;
}




