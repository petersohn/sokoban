#ifndef STATUS_H_
#define STATUS_H_

#include "Array.h"
#include "Problem.h"

class Status {
	Problem::Ptr problem;
	VisitedState state;
	Array<bool> reachable;
	Array<uint> stoneAt;
	Array<Problem::FieldType> fields;
public:
	Status();
};

#endif /* STATUS_H_ */
