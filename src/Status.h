#ifndef STATUS_H_
#define STATUS_H_

#include "Common.h"
#include "Array.h"
#include "Problem.h"
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <vector>

class Status {
public:
	typedef boost::shared_ptr<Status> Ptr;
//	typedef boost::function<bool(Status&)> CheckFunction;
private:
	Problem::ConstPtr problem_;
	VisitedState state_;
	Array<bool> reachable_;
	Array<uint> stoneAt_;
	Array<FieldType> fields_;
	bool reachOK_;
	std::vector<CheckFunction> checkFunctions_;

	void calculateStoneAt();
	void calculateReachable();
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
	FieldType fields(const Point &p) const { return arrayAt(fields_, p); }
	bool moveStone(int stone, const Point & p);
//	const std::vector<CheckFunction>& checkFunctions() const { return checkFunctions_; }
//	std::vector<CheckFunction>& checkFunctions() { return checkFunctions_; }
};

#endif /* STATUS_H_ */
