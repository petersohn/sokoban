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
public:
	Status(const Problem::ConstPtr &problem);
	Status(const Status &other);

};

#endif /* STATUS_H_ */
