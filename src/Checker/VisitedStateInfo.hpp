#ifndef VISITEDSTATE_H_
#define VISITEDSTATE_H_

#include "Status/Point.hpp"
#include "Status/State.hpp"
#include "Hash.hpp"

namespace sokoban {

class Status;

class VisitedStateInfo {
    State state_;
    Point firstReachable_;
public:
    VisitedStateInfo() = default;
    explicit VisitedStateInfo(const Status& status);

    const State& state() const { return state_; }
    Point firstReachable() const { return firstReachable_; }

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int /*version*/)
    {
        ar & state_;
        ar & firstReachable_;
    }
};

bool operator==(const VisitedStateInfo& left, const VisitedStateInfo& right);

inline
bool operator!=(const VisitedStateInfo& left, const VisitedStateInfo& right) {
    return !(left == right);
}

} // namespace sokoban

namespace std {

template<>
struct hash<sokoban::VisitedStateInfo> {
    size_t operator()(const sokoban::VisitedStateInfo& state) const
    {
        size_t seed = 0;
        sokoban::hash_combine(seed, state.state());
        sokoban::hash_combine(seed, state.firstReachable());
        return seed;
    }
};

} // namespace std

#endif /* VISITEDSTATE_H_ */
