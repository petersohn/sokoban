#ifndef PROBLEM_H_
#define PROBLEM_H_

#include "Common.h"
#include "Array.h"
#include "Table.h"
#include <boost/shared_ptr.hpp>

class VisitedState;

class Problem {
public:
	typedef boost::shared_ptr<Problem> Ptr;
private:
	size_t stoneNum_;
	Array<bool> stones_;
	Point startPos_;
	VisitedState beginningState_;
	bool stateOK_;
	FixedTable::Ptr table_;

	void initState();
public:
	Problem();
	explicit Problem(const char *filename) {
		reload(filename);
	}
	explicit Problem(FixedTable::Ptr table) {
		reset(table);
	}

	size_t width() const { return table_.width(); }
	size_t height() const { return table_.height(); }
	size_t stoneNum() const { return stoneNum_; }
	const Table& table() { return table_->get(); }
	FixedTable::Ptr tablePtr() { return table_; }
	FieldType value(const Point &p) const {
		return (value().wall(p) ? ftWall :
				(stones_[p] ? ftStone : ftFloor));
	}
	FieldType tableValue(const Point &p) const {
		return value().wall(p) ? ftWall : ftFloor;
	}
	const VisitedState& beginningState() const {
		if (!visitedStateOK())
			initState();
		return beginningState_;
	}
	const Point& startPos() const { return startPos_; }
	const Point& destination() const { return table().destination(); }

	void reload(const char *filename);
	void reset(FixedTable::Ptr table);
	void clearStones();
	void startPos(const Point &p);
	bool addStone(const Point &p);
	bool removeStone(const Point &p);
};

typedef FixedObject<Problem> FixedProblem;

#endif /* PROBLEM_H_ */
