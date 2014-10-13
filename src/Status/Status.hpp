#ifndef STATUS_H_
#define STATUS_H_

#include "Common.hpp"
#include "Array.hpp"
#include "Status/Table.hpp"
#include "Status/State.hpp"
#include <boost/thread/mutex.hpp>
#include <memory>
#include <functional>
#include <vector>
#include <deque>
#include <unordered_map>

class Status {
public:
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

	typedef std::shared_ptr<CalculatedData> CalculatedDataPtr;

	mutable  CalculatedDataPtr calculatedData_;

	static std::size_t statusPoolSize_;
	static std::unordered_map<State, std::shared_ptr<Array<CalculatedDataPtr>>> statusPool_;
	static boost::mutex statusPoolMutex_;

	void fillReachable() const;
	void calculateReachable() const;
	void init();
	bool shiftIter(Point p);

public:
	static std::size_t copyCount;
	static std::size_t moveCount;
	static std::size_t calculateReachableCount;

	explicit Status(FixedTable::Ptr table);
	explicit Status(FixedTable::Ptr table, const State &state);
	explicit Status(FixedTable::Ptr table, const Node &node);
	Status(const Status& other):
		table_(other.table_),
		state_(other.state_),
		currentPos_(other.currentPos_),
		fields_(other.fields_),
		calculatedData_(other.calculatedData_)
	{
		++copyCount;
	}
	Status& operator=(const Status& other) {
		table_ = other.table_;
		state_ = other.state_;
		currentPos_ = other.currentPos_;
		fields_ = other.fields_;
		calculatedData_ = other.calculatedData_;
		++copyCount;
		return *this;
	}

	Status(Status&& other) noexcept:
		table_(std::move(other.table_)),
		state_(std::move(other.state_)),
		currentPos_(std::move(other.currentPos_)),
		fields_(std::move(other.fields_)),
		calculatedData_(std::move(other.calculatedData_))
	{
		++moveCount;
	}
	Status& operator=(Status&& other) noexcept {
		table_ = std::move(other.table_);
		state_ = std::move(other.state_);
		currentPos_ = std::move(other.currentPos_);
		fields_ = std::move(other.fields_);
		calculatedData_ = std::move(other.calculatedData_);
		++moveCount;
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
	std::size_t width() const { return table().width(); }
	std::size_t height() const { return table().height(); }
	const State& state() const { return state_; }
	bool reachable(Point p) const {
		if (!calculatedData_)
			fillReachable();
		return arrayAt<bool>(calculatedData_->reachable_, p, false);
	}
	const Array<bool>& reachableArray() const {
		if (!calculatedData_)
			fillReachable();
		return calculatedData_->reachable_;
	}
	const BorderType& border() const {
		if (!calculatedData_)
			fillReachable();
		return calculatedData_->border_;
	}
	FieldType value(Point p) const { return arrayAt<FieldType>(fields_, p, ftWall); }
	Point currentPos() const { return currentPos_; }

	bool currentPos(Point p);
	void state(const State &value);
	bool addStone(Point p);
	bool removeStone(Point p);
	bool moveStone(Point from, Point to);
	void set(const Node &node);
	void shiftCurrentPos();

	static void statusPoolSize(std::size_t value) {
		statusPoolSize_ = value;
	}
	static std::size_t statusPoolSize() { return statusPoolSize_; }
	static std::size_t currentStatusPoolSize() { return statusPool_.size(); }
};

inline bool operator!=(const Status& lhs, const Status& rhs)
{
	return !(lhs == rhs);
}

#endif /* STATUS_H_ */
