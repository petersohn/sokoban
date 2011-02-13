#ifndef STATUS_H_
#define STATUS_H_

#include "Common.h"
#include "Array.h"
#include "Table.h"
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <vector>
#include <deque>

class Status {
public:
	typedef boost::shared_ptr<Status> Ptr;
	typedef std::deque<int> BorderType;
private:
	FixedTable::Ptr table_;
	VisitedState state_;
	Array<uint> stoneAt_;
	Array<FieldType> fields_;

	mutable Array<bool> reachable_;
	mutable BorderType border_;
	mutable bool reachOK_;

	void calculateReachable() const;
	void init();
public:
	explicit Status(FixedTable::Ptr table, const VisitedState &state);
	explicit Status(FixedTable::Ptr table, const Node &node);

	const Table& table() const { return table_->get(); }
	FixedTable::Ptr tablePtr() const { return table_; }
	const VisitedState& state() const { return state_; }
	bool reachable(const Point &p) const {
		if (!reachOK_)
			calculateReachable();
		return reachable_[p];
	}
	const BorderType& border() const {
		if (!reachOK_)
			calculateReachable();
		return border_;
	}
	uint stoneAt(const Point &p) const { return stoneAt_[p]; }
	FieldType value(const Point &p) const { return arrayAt<FieldType>(fields_, p, ftWall); }

	void state(const VisitedState &value);
	bool addStone(const Point &p);
	bool removeStone(const Point &p);
	bool moveStone(int stone, const Point & p);
};

#endif /* STATUS_H_ */
