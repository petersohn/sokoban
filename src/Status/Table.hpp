#ifndef TABLE_H_
#define TABLE_H_

#include "Array.hpp"

#include <memory>

class Table {
private:
    Array<bool> walls_;
    Point startingPoint_;
    Point destination_;
public:
    Table() {}
    Table(size_t width, size_t height):walls_(width, height, true) {}

    size_t width() const { return walls_.width(); }
    size_t height() const { return walls_.height(); }
    bool wall(Point p) const {
        return arrayAt<bool>(walls_, p, true);
    }
    Point  destination() const { return destination_; }
    Point  startingPoint() const { return startingPoint_; }

    void wall(Point p, bool value) {
        walls_[p] = value;
    }
    void destination(Point  p) {
        assert(isInsideArray(walls_, p));
        destination_ = p;
    }
    void startingPoint(Point  p) {
        assert(isInsideArray(walls_, p));
        startingPoint_ = p;
    }
};


#endif /* TABLE_H_ */
