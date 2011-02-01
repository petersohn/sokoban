#ifndef PROBLEM_H_
#define PROBLEM_H_

#include "Array.h"
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

class VisitedState;

class Problem:public boost::noncopyable {
public:
	enum FieldType { ftWall, ftFloor, ftStone };
	typedef boost::shared_ptr<Problem> Ptr;
	typedef boost::shared_ptr<const Problem> ConstPtr;
private:
	size_t stoneNum_;
	Array<FieldType> table_;
	VisitedState beginningState_;
	Point destination_;

	void initState();
public:
	Problem();
	Problem(const char *filename) {
		reload(filename);
	}
	void reload(const char *filename);

	size_t width() const { return table_.width(); }
	size_t height() const { return table_.height(); }
	size_t stoneNum() const { return stoneNum_; }
	FieldType table(const Point &p) const { return table_[p]; }
	const VisitedState& beginningState() const { return beginningState_; }
};

#endif /* PROBLEM_H_ */
