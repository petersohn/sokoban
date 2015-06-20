#ifndef PSEUDOSTATUS_H_
#define PSEUDOSTATUS_H_

#include "Status/Status.hpp"
#include "FieldType.hpp"

#include <boost/serialization/vector.hpp>

class PseudoStatus {
    const Table* table_;
    State state_;
    Point currentPos_;
    Array<bool> reachableArray_;
public:
    PseudoStatus(): table_{nullptr} {}
    explicit PseudoStatus(const Status& status):
        table_(&status.table()),
        state_(status.state()),
        currentPos_(status.currentPos()),
        reachableArray_(status.reachableArray())
    {}

    size_t width() const { return table().width(); }
    size_t height() const { return table().height(); }
    const Table& table() const { return *table_; }
    State& state() { return state_; }
    const State& state() const { return state_; }
    FieldType value(Point p) const {
        return table().wall(p) ? FieldType::wall :
                (state()[p] ? FieldType::stone : FieldType::floor);
    }
    Point& currentPos() { return currentPos_; }
    Point  currentPos() const { return currentPos_; }
    bool reachable(Point  p) const { return arrayAt<bool>(reachableArray_, p, false); }
    Array<bool>& reachableArray() { return reachableArray_; }
    const Array<bool>& reachableArray() const { return reachableArray_; }

    template <typename Ar>
    void serialize(Ar& ar, const unsigned int)
    {
        ar & table_;
        ar & state_;
        ar & currentPos_;
        ar & reachableArray_;
    }
};


#endif /* PSEUDOSTATUS_H_ */
