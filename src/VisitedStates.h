#ifndef VISITEDSTATES_H_
#define VISITEDSTATES_H_

#include "QueueInterfaces.h"
#include "State.h"
#include "VisitedStateInfo.h"
#include "Status.h"
#include <boost/unordered_map.hpp>


class VisitedStates: public VisitedStateHolder {
	typedef boost::unordered_multimap<State, VisitedStateInfo, boost::hash<State> > VisitedStateSet;
	VisitedStateSet visitedStates_;
public:
	virtual void push(const VisitedStateInput &elem);
	virtual bool hasElem(const VisitedStateInput &elem) const;
	virtual size_t size();
};

#endif /* VISITEDSTATES_H_ */
