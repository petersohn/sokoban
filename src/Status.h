#ifndef STATUS_H_
#define STATUS_H_

#include "Common.h"
#include "Array.h"
#include "Table.h"
#include "State.h"
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
	static Status loadFromFile(const char *filename);

	explicit Status(FixedTable::Ptr table);
	explicit Status(FixedTable::Ptr table, const VisitedState &state);
	explicit Status(FixedTable::Ptr table, const Node &node);

	const Table& table() const { return table_->get(); }
	FixedTable::Ptr tablePtr() const { return table_; }
	size_t width() const { return table().width(); }
	size_t height() const { return table().height(); }
	const VisitedState& state() const { return state_; }
	bool reachable(const Point &p) const {
		if (!reachOK_)
			calculateReachable();
		return arrayAt<bool>(reachable_, p, false);
	}
	const BorderType& border() const {
		if (!reachOK_)
			calculateReachable();
		return border_;
	}
	uint stoneAt(const Point &p) const { return stoneAt_[p]; }
	FieldType value(const Point &p) const { return arrayAt<FieldType>(fields_, p, ftWall); }
	const Point &currentPos() const { return state_.currentPos(); }

	bool currentPos(const Point &p);
	void state(const VisitedState &value);
	bool addStone(const Point &p);
	bool removeStone(const Point &p);
	bool moveStone(int stone, const Point & p);
};

void floodFill(const Status &table, const Point &p0, Array<bool> &result,
			std::deque<int> *border = NULL, MinMax *minmax = NULL);

void shiftCurrentPos(Status &status);

#endif /* STATUS_H_ */
