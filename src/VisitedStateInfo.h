#ifndef VISITEDSTATE_H_
#define VISITEDSTATE_H_

#include "Status/Point.h"
#include "Status/State.h"
#include "Hash.h"

class Status;

class VisitedStateInfo {
	State state_;
	Point firstReachable_;
public:
	VisitedStateInfo() {}
	VisitedStateInfo(const Status& status);

	const State& state() const { return state_; }
	const Point& firstReachable() const { return firstReachable_; }
};

bool operator==(const VisitedStateInfo& left, const VisitedStateInfo& right);

inline
bool operator!=(const VisitedStateInfo& left, const VisitedStateInfo& right) {
	return !(left == right);
}

namespace std {

template<>
struct hash<VisitedStateInfo> {
	size_t operator()(const VisitedStateInfo& state) const
	{
		size_t seed = 0;
		hash_combine(seed, state.state());
		hash_combine(seed, state.firstReachable());
		return seed;
	}
};

}

#endif /* VISITEDSTATE_H_ */
