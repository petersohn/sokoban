#ifndef STATUS_H_
#define STATUS_H_

#include "Array.h"
#include "Problem.h"

class Status {
	Problem::ConstPtr problem_;
	VisitedState state_;
	Array<bool> reachable_;
	Array<uint> stoneAt_;
	Array<Problem::FieldType> fields_;
	void copy(const Status &other);
	int reachOK;

	void calculateReachable();
	void moveStone(int stone, const Point & p);
public:
	Status(const Problem &problem);
	Status(const Status &other) {
		copy(other);
	}
	Status& operator=(const Status &other) {
		copy(other);
		return *this;
	}

	Problem::ConstPtr problem() const { return problem_; }
	const VisitedState& state() const { return state_; }
	bool reachable(const Point &p) const {
		if (!reachOK)
			calculateReachable();
		return reachable_[p];
	}
	uint stoneAt(const Point &p) const { return stoneAt_[p]; }
	Problem::FieldType fields(const Point &p) const { return fields_[p]; }

	bool step(int stone, const Point &d);
};

#endif /* STATUS_H_ */
