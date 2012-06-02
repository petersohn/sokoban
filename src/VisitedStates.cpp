#include "VisitedStates.h"
#include <boost/foreach.hpp>

void VisitedStates::push(const VisitedStateInput &elem)
{
	const Status &st = elem.first;
	visitedStates_.insert(std::make_pair(st.state(), VisitedStateInfo(st.currentPos(), elem.second)));
}

bool VisitedStates::hasElem(const VisitedStateInput &elem) const
{
	const Status &status = elem.first;
	int heur = elem.second;
	BOOST_FOREACH(const VisitedStateSet::value_type& value,
			visitedStates_.equal_range(status.state())) {
		if (status.reachable(value.second.currentPos()) && heur >= value.second.heur()) {
			return true;
		}
	}
	return false;
}

bool VisitedStates::checkAndPush(const VisitedStateInput &elem)
{
	boost::lock_guard<MutexType> lck(visitedStatesMutex_);
	if (hasElem(elem))
		return false;
	push(elem);
	return true;
}

size_t VisitedStates::size() const {
	boost::lock_guard<MutexType> lck(visitedStatesMutex_);
	return visitedStates_.size();
}
