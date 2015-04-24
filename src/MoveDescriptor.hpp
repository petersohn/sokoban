#ifndef MOVEDESCRIPTOR_H_
#define MOVEDESCRIPTOR_H_

#include "Status/Point.hpp"

struct MoveDescriptor {
    Point from_;
    Point d_;
    MoveDescriptor(Point  p, Point  d):
        from_(p), d_(d) {}
};



#endif /* MOVEDESCRIPTOR_H_ */
