#ifndef PROBLEM_H_
#define PROBLEM_H_

#include "Array.h"
#include <boost/shared_ptr.hpp>

class VisitedState;

class Problem {
public:
	typedef boost::shared_ptr<Problem> Ptr;
	typedef boost::shared_ptr<const Problem> ConstPtr;
private:
	size_t stoneNum_;
	Array<FieldType> table_;
	VisitedState beginningState_;
	bool stateOK_;
	Point destination_;

	void initState();
public:
	Problem();
	explicit Problem(const char *filename) {
		reload(filename);
	}

	size_t width() const { return table_.width(); }
	size_t height() const { return table_.height(); }
	size_t stoneNum() const { return stoneNum_; }
	FieldType table(const Point &p) const {
		if (p.x >= 0 && p.y >= 0 && p.x < width() && p.y < height())
			return table_[p];
		return ftWall;
	}
	FieldType bareTable(const Point &p) const {
		if (p.x >= 0 && p.y >= 0 && p.x < width() && p.y < height())
			return table_[p] == ftStone ? ftFloor : table_[p];
		return ftWall;
	}
	const VisitedState& beginningState() const {
		if (!visitedStateOK())
			initState();
		return beginningState_;
	}

	void reload(const char *filename);
	void clearStones();
	void setTable(const Point &p, FieldType f);
	bool addStone(const Point &p);
	bool removeStone(const Point &p);
};

#endif /* PROBLEM_H_ */
