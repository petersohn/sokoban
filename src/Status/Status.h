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
#include <unordered_set>

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

	static bool enableStatusPooling_;
	static std::unordered_set<Status> statusPool_;

	struct CalculatedData {
		Array<bool> reachable_;
		BorderType border_;

		CalculatedData(std::size_t width, std::size_t height):
			reachable_(width, height, false)
		{}
	};

	mutable std::shared_ptr<CalculatedData> calculatedData_;

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
		calculatedData_(other.calculatedData_)
	{
		++copyCount;
	}

	Status& operator=(const Status &other) {
		table_=other.table_;
		state_=other.state_;
		currentPos_=other.currentPos_;
		fields_=other.fields_;
		calculatedData_ = other.calculatedData_;
		++copyCount;
		return *this;
	}

	bool operator==(const Status& other) const
	{
		if (table_ != other.table_ || state_ != other.state_) {
			return false;
		}

		if (calculatedData_) {
			return reachable(other.currentPos_);
		} else {
			return other.reachable(currentPos_);
		}
	}

	const Table& table() const { return table_->get(); }
	FixedTable::Ptr tablePtr() const { return table_; }
	size_t width() const { return table().width(); }
	size_t height() const { return table().height(); }
	const State& state() const { return state_; }
	bool reachable(const Point &p) const {
		if (!calculatedData_)
			calculateReachable();
		return arrayAt<bool>(calculatedData_->reachable_, p, false);
	}
	const Array<bool>& reachableArray() const {
		if (!calculatedData_)
			calculateReachable();
		return calculatedData_->reachable_;
	}
	const BorderType& border() const {
		if (!calculatedData_)
			calculateReachable();
		return calculatedData_->border_;
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

	static void enableStatusPooling(bool value) {
		enableStatusPooling_ = value;
	}
	bool enableStatusPooling() { return enableStatusPooling_; }
};

inline bool operator!=(const Status& lhs, const Status& rhs)
{
	return !(lhs == rhs);
}


namespace std {

template<>
struct hash<Status> {
	size_t operator()(const Status &status) const
	{
		size_t seed = 0;
		hash_combine(seed, status.tablePtr());
		hash_combine(seed, status.state());
		return seed;
	}
};


}


#endif /* STATUS_H_ */
