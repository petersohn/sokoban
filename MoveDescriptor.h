#ifndef MOVEDESCRIPTOR_H_
#define MOVEDESCRIPTOR_H_

#include "Status/Point.h"

struct MoveDescriptor {
	Point from_;
	Point d_;
	MoveDescriptor(const Point& p, const Point& d):
		from_(p), d_(d) {}
};



#endif /* MOVEDESCRIPTOR_H_ */
