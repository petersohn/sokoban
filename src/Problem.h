#ifndef PROBLEM_H_
#define PROBLEM_H_

#include "Common.h"
#include "Array.h"
#include <boost/shared_ptr.hpp>

class VisitedState;

class Problem {
public:
//	typedef boost::shared_ptr<Problem> Ptr;
//	typedef boost::shared_ptr<const Problem> ConstPtr;
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
		return arrayAt<FieldType, ftWall>(table_, p);
	}
	FieldType bareTable(const Point &p) const {
		return arrayAt<FieldType, ftWall>(table_, p) == ftStone ?
				ftFloor :
				arrayAt<FieldType, ftWall>(table_, p);
	}
	const VisitedState& beginningState() const {
		if (!visitedStateOK())
			initState();
		return beginningState_;
	}
	const Point& destination() { return destination_; }

	void reload(const char *filename);
	void clearStones();
	void setTable(const Point &p, FieldType f);
	bool addStone(const Point &p);
	bool removeStone(const Point &p);
};

class FixedProblem {
	Problem problem_;
public:
	typedef boost::shared_ptr<const FixedProblem> Ptr;

	FixedProblem(const Problem &p): problem_(p) {}
	const Problem &problem() const { return problem_; }
};

#endif /* PROBLEM_H_ */
