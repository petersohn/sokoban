#ifndef VISITEDSTATES_H_
#define VISITEDSTATES_H_

#include "QueueInterfaces.hpp"
#include "Status/State.hpp"
#include "VisitedStateInfo.hpp"
#include "Status/Status.hpp"
#include "Mutexes.hpp"
#include <unordered_map>

typedef std::pair<const Status&, int> VisitedStateInput;

class VisitedStates {
	typedef std::unordered_map<VisitedStateInfo, int, std::hash<VisitedStateInfo>> VisitedStateSet;
	VisitedStateSet visitedStates_;
public:
	VisitedStates() = default;
	bool checkAndPush(const VisitedStateInput &elem);
	size_t size() const { return visitedStates_.size(); }
	bool empty() const { return visitedStates_.empty(); }
};

#endif /* VISITEDSTATES_H_ */
