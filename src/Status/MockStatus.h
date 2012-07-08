#ifndef MOCKSTATUS_H_
#define MOCKSTATUS_H_

#include "Status/Status.h"

class MockStatus {
	State state_;
	Point currentPos_;
	Array<bool> reachableArray_;
public:
	explicit MockStatus(const Status& status):
		state_(status.state()),
		currentPos_(status.currentPos()),
		reachableArray_(status.reachableArray())
	{}
	State& state() { return state_; }
	const State& state() const { return state_; }
	Point& currentPos() { return currentPos_; }
	const Point& currentPos() const { return currentPos_; }
	bool reachable(const Point& p) const { return arrayAt<bool>(reachableArray_, p, false); }
	Array<bool>& reachableArray() { return reachableArray_; }
	const Array<bool>& reachableArray() const { return reachableArray_; }
};


#endif /* MOCKSTATUS_H_ */
