#ifndef PSEUDOSTATUS_H_
#define PSEUDOSTATUS_H_

#include "Status/Status.hpp"

class PseudoStatus {
	FixedTable::Ptr table_;
	State state_;
	Point currentPos_;
	Array<bool> reachableArray_;
public:
	explicit PseudoStatus(const Status& status):
		table_(status.tablePtr()),
		state_(status.state()),
		currentPos_(status.currentPos()),
		reachableArray_(status.reachableArray())
	{}

	size_t width() const { return table().width(); }
	size_t height() const { return table().height(); }
	const Table& table() const { return table_->get(); }
	const FixedTable::Ptr& tablePtr() const { return table_; }
	State& state() { return state_; }
	const State& state() const { return state_; }
	FieldType value(Point p) const {
		return table().wall(p) ? ftWall :
				(state()[p] ? ftStone : ftFloor);
	}
	Point& currentPos() { return currentPos_; }
	Point  currentPos() const { return currentPos_; }
	bool reachable(Point  p) const { return arrayAt<bool>(reachableArray_, p, false); }
	Array<bool>& reachableArray() { return reachableArray_; }
	const Array<bool>& reachableArray() const { return reachableArray_; }
};


#endif /* PSEUDOSTATUS_H_ */