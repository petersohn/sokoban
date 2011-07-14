#ifndef VISITEDSTATES_H_
#define VISITEDSTATES_H_

#include "QueueInterfaces.h"
#include "State.h"
#include "VisitedStateInfo.h"
#include "Status.h"
#include "Common.h"
#include <boost/unordered_map.hpp>

class VisitedStates: public VisitedStateHolder {
	typedef boost::unordered_multimap<State, VisitedStateInfo, boost::hash<State> > VisitedStateSet;
	VisitedStateSet visitedStates_;
	mutable MutexType mtx_;
	void push(const VisitedStateInput &elem);
	bool hasElem(const VisitedStateInput &elem) const;
public:
	virtual bool checkAndPush(const VisitedStateInput &elem);
	virtual size_t size() const;
};

#endif /* VISITEDSTATES_H_ */
