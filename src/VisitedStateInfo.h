#ifndef VISITEDSTATE_H_
#define VISITEDSTATE_H_

#include "Point.h"
#include "State.h"

class Status;

class VisitedStateInfo {
	State state_;
	Point firstReachable_;
public:
	VisitedStateInfo(const Status& status);

	const State& state() const { return state_; }
	const Point& firstReachable() const { return firstReachable_; }
};

bool operator==(const VisitedStateInfo& left, const VisitedStateInfo& right);

inline
bool operator!=(const VisitedStateInfo& left, const VisitedStateInfo& right) {
	return !(left == right);
}

size_t hash_value(const VisitedStateInfo& state);


#endif /* VISITEDSTATE_H_ */
