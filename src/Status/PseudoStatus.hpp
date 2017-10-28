#ifndef PSEUDOSTATUS_H_
#define PSEUDOSTATUS_H_

#include "Status/Status.hpp"
#include "FieldType.hpp"

#include <boost/serialization/vector.hpp>

namespace sokoban {

class PseudoStatus {
    const Table* table_;
    State state_;
    Point currentPos_;
    Matrix<bool> reachableMatrix_;
public:
    PseudoStatus(): table_{nullptr} {}
    explicit PseudoStatus(const Status& status):
        table_(&status.table()),
        state_(status.state()),
        currentPos_(status.currentPos()),
        reachableMatrix_(status.reachableMatrix())
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
    bool reachable(Point  p) const { return matrixAt<bool>(reachableMatrix_, p, false); }
    Matrix<bool>& reachableMatrix() { return reachableMatrix_; }
    const Matrix<bool>& reachableMatrix() const { return reachableMatrix_; }

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int)
    {
        ar & table_;
        ar & state_;
        ar & currentPos_;
        ar & reachableMatrix_;
    }
};

} // namespace sokoban

#endif /* PSEUDOSTATUS_H_ */
