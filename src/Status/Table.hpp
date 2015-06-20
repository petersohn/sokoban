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
    Table() = default;
    Table(size_t width, size_t height):walls_(width, height, true) {}

    Table(const Table&) = default;
    Table& operator=(const Table&) = default;
    Table(Table&&) = default;
    Table& operator=(Table&&) = default;

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

    bool operator==(const Table& other) const {
        return walls_ == other.walls_ &&
                startingPoint_ == other.startingPoint_ &&
                destination_ == other.destination_;
    }

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int /*version*/) {
        ar & walls_;
        ar & startingPoint_;
        ar & destination_;
    }
};

inline
bool operator!=(const Table& lhs, const Table& rhs) {
    return !(lhs == rhs);
}

#endif /* TABLE_H_ */
