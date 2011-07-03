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
	typedef boost::shared_ptr<const Status> ConstPtr;
	typedef std::deque<Point> BorderType;
private:
	FixedTable::Ptr table_;
	State state_;
	Point currentPos_;
	Array<FieldType> fields_;

	mutable Array<bool> reachable_;
	mutable BorderType border_;
	mutable bool reachOK_;

	void calculateReachable() const;
	void init();
public:
	static int copyCount;
	static Status loadFromFile(const char *filename);

	explicit Status(FixedTable::Ptr table);
	explicit Status(FixedTable::Ptr table, const State &state);
	explicit Status(FixedTable::Ptr table, const Node &node);
	Status(const Status &other):
		table_(other.table_),
		state_(other.state_),
		currentPos_(other.currentPos_),
		fields_(other.fields_),
//		reachable_(other.reachable_),
//		border_(other.border_),
		reachOK_(false)
	{
		++copyCount;
	}

	Status& operator=(const Status &other) {
		table_=other.table_;
		state_=other.state_;
		currentPos_=other.currentPos_;
		fields_=other.fields_;
//		reachable_=other.reachable_;
//		border_=other.border_;
		reachOK_=false;
		++copyCount;
	}

	const Table& table() const { return table_->get(); }
	FixedTable::Ptr tablePtr() const { return table_; }
	size_t width() const { return table().width(); }
	size_t height() const { return table().height(); }
	const State& state() const { return state_; }
	bool reachable(const Point &p) const {
		if (!reachOK_)
			calculateReachable();
		return arrayAt<bool>(reachable_, p, false);
	}
	const Array<bool>& reachableArray() {
		if (!reachOK_)
			calculateReachable();
		return reachable_;
	}
	const BorderType& border() const {
		if (!reachOK_)
			calculateReachable();
		return border_;
	}
	FieldType value(const Point &p) const { return arrayAt<FieldType>(fields_, p, ftWall); }
	const Point &currentPos() const { return currentPos_; }

	bool currentPos(const Point &p);
	void state(const State &value);
	bool addStone(const Point &p);
	bool removeStone(const Point &p);
	bool moveStone(const Point &from, const Point &to);
	void set(const Node &node);
};

void floodFill(const Status &table, const Point &p0, Array<bool> &result,
			Status::BorderType *border = NULL, MinMax *minmax = NULL);

void shiftCurrentPos(Status &status);

std::vector<Status::Ptr> getPartitions(FixedTable::Ptr table, const State &state);

#endif /* STATUS_H_ */
