#ifndef STATUS_H_
#define STATUS_H_

#include "Common.h"
#include "Array.h"
#include "Status/Table.h"
#include "Status/State.h"
#include <memory>
#include <functional>
#include <vector>
#include <deque>

class Status {
public:
	typedef std::shared_ptr<Status> Ptr;
	typedef std::shared_ptr<const Status> ConstPtr;
	typedef std::vector<Point> BorderType;
private:
	FixedTable::Ptr table_;
	State state_;
	Point currentPos_;
	Array<FieldType> fields_;

	struct CalculatedData {
		Array<bool> reachable_;
		BorderType border_;

		CalculatedData(std::size_t width, std::size_t height):
			reachable_(width, height, false)
		{}
	};

	mutable std::shared_ptr<CalculatedData> calculatedData;

	void calculateReachable() const;
	void init();
	bool shiftIter(const Point &p);

public:
	static int copyCount;
	static int calculateReachableCount;

	explicit Status(FixedTable::Ptr table);
	explicit Status(FixedTable::Ptr table, const State &state);
	explicit Status(FixedTable::Ptr table, const Node &node);
	Status(const Status &other):
		table_(other.table_),
		state_(other.state_),
		currentPos_(other.currentPos_),
		fields_(other.fields_),
		calculatedData(other.calculatedData)
	{
		++copyCount;
	}

	Status& operator=(const Status &other) {
		table_=other.table_;
		state_=other.state_;
		currentPos_=other.currentPos_;
		fields_=other.fields_;
		calculatedData = other.calculatedData;
		++copyCount;
		return *this;
	}

	const Table& table() const { return table_->get(); }
	FixedTable::Ptr tablePtr() const { return table_; }
	size_t width() const { return table().width(); }
	size_t height() const { return table().height(); }
	const State& state() const { return state_; }
	bool reachable(const Point &p) const {
		if (!calculatedData)
			calculateReachable();
		return arrayAt<bool>(calculatedData->reachable_, p, false);
	}
	const Array<bool>& reachableArray() const {
		if (!calculatedData)
			calculateReachable();
		return calculatedData->reachable_;
	}
	const BorderType& border() const {
		if (!calculatedData)
			calculateReachable();
		return calculatedData->border_;
	}
	FieldType value(const Point &p) const { return arrayAt<FieldType>(fields_, p, ftWall); }
	const Point &currentPos() const { return currentPos_; }

	bool currentPos(const Point &p);
	void state(const State &value);
	bool addStone(const Point &p);
	bool removeStone(const Point &p);
	bool moveStone(const Point &from, const Point &to);
	void set(const Node &node);
	void shiftCurrentPos();
};

#endif /* STATUS_H_ */
