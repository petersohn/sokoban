#ifndef VISITEDSTATES_H_
#define VISITEDSTATES_H_

#include "QueueInterfaces.h"
#include "State.h"
#include "VisitedStateInfo.h"
#include "Status.h"
#include "Common.h"
#include "Mutexes.hpp"
#include <unordered_map>

class VisitedStates: public VisitedStateHolder {
	typedef std::unordered_map<VisitedStateInfo, int, std::hash<VisitedStateInfo> > VisitedStateSet;
	VisitedStateSet visitedStates_;
	mutable MutexType visitedStatesMutex_;
public:
	VisitedStates():
		MUTEX_DECL(visitedStatesMutex_)
	{}
	virtual bool checkAndPush(const VisitedStateInput &elem);
	virtual size_t size() const;
};

#endif /* VISITEDSTATES_H_ */
