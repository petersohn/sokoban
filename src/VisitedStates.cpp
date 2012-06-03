#include "VisitedStates.h"
#include <boost/foreach.hpp>

bool VisitedStates::checkAndPush(const VisitedStateInput &elem)
{
	boost::lock_guard<MutexType> lck(visitedStatesMutex_);
	const Status &status = elem.first;
	int heur = elem.second;
	VisitedStateInfo info(status);
	VisitedStateSet::iterator it = visitedStates_.find(info);
	if (it == visitedStates_.end()) {
		visitedStates_.insert(std::make_pair(info, heur));
		return true;
	}
	if (it->second > heur) {
		it->second = heur;
		return true;
	}
	return false;
}

size_t VisitedStates::size() const {
	boost::lock_guard<MutexType> lck(visitedStatesMutex_);
	return visitedStates_.size();
}
