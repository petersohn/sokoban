#ifndef MOVEDESCRIPTOR_H_
#define MOVEDESCRIPTOR_H_

#include "Status/Point.hpp"

struct MoveDescriptor {
    Point from_;
    Point d_;
    MoveDescriptor(Point  p, Point  d):
        from_(p), d_(d) {}
};

template <typename Archive>
void serialize(Archive& ar, MoveDescriptor& moveDescriptor, 
        const unsigned int /*version*/) {
    ar & moveDescriptor.from_;
    ar & moveDescriptor.d_;
}

#endif /* MOVEDESCRIPTOR_H_ */
