#ifndef STATUS_H_
#define STATUS_H_

#include "Array.h"
#include "Problem.h"
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <vector>

class Status {
public:
	typedef boost::shared_ptr<Status> Ptr;
	typedef boost::function<bool(Status&)> CheckFunction;
private:
	Problem::ConstPtr problem_;
	VisitedState state_;
	Array<bool> reachable_;
	Array<uint> stoneAt_;
	Array<Problem::FieldType> fields_;
	bool reachOK_;
	std::vector<CheckFunction> checkFunctions_;

	void calculateStoneAt();
	void calculateReachable();
	bool moveStone(int stone, const Point & p);
public:
	Status(const Problem &problem);

	Problem::ConstPtr problem() const { return problem_; }
	const VisitedState& state() const { return state_; }
	bool reachable(const Point &p) const {
		if (!reachOK_)
			calculateReachable();
		return reachable_[p];
	}
	uint stoneAt(const Point &p) const { return stoneAt_[p]; }
	Problem::FieldType fields(const Point &p) const { return fields_[p]; }
	const std::vector<CheckFunction>& checkFunctions() const { return checkFunctions_; }
	std::vector<CheckFunction>& checkFunctions() { return checkFunctions_; }

	bool step(int stone, const Point &d);
};

#endif /* STATUS_H_ */
