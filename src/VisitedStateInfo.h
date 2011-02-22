#ifndef VISITEDSTATE_H_
#define VISITEDSTATE_H_

#include "Point.h"

class Status;

class VisitedStateInfo {
	Point currentPos_;
	int heur_;
public:
	VisitedStateInfo(const Point &currentPos, int heur);

	const Point& currentPos() const { return currentPos_; }
	int heur() const { return heur_; }
};


#endif /* VISITEDSTATE_H_ */
