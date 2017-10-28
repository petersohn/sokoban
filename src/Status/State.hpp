#ifndef STATE_H_
#define STATE_H_

#include "Status/Point.hpp"

#include <boost/range/algorithm.hpp>
#include <boost/serialization/vector.hpp>

#include <algorithm>
#include <assert.h>
#include <functional>
#include <iterator>
#include <memory>
#include <unordered_set>

namespace sokoban {

class State {
    typedef std::unordered_set<Point> ContainerType;
    typedef std::shared_ptr<ContainerType> ContainerPtr;
    ContainerPtr stones_;
    void modify() {
        if (!stones_.unique())
            stones_ = std::make_shared<ContainerType>(*stones_);
    }
public:
    typedef ContainerType::const_iterator const_iterator;

    State(): stones_(std::make_shared<ContainerType>()) {}
    State(const State&) = default;
    State& operator=(const State&) = default;
    State(State&&) = default;
    State& operator=(State&&) = default;

    explicit State(std::initializer_list<Point> range):
            stones_(std::make_shared<ContainerType>())
    {
        std::move(std::begin(range), std::end(range),
                std::inserter(*stones_, stones_->begin()));
    }

    template <typename Range>
    explicit State(Range range):
            stones_(std::make_shared<ContainerType>())
    {
        std::move(std::begin(range), std::end(range),
                std::inserter(*stones_, stones_->begin()));
    }

    explicit State(ContainerType range):
        stones_{std::make_shared<ContainerType>(std::move(range))}
    {
    }


    State deepCopy() const {
        State result{*this};
        result.modify();
        return result;
    }

    bool operator==(const State& other) const {
        return stones_ == other.stones_ || *stones_ == *(other.stones_);
    }
    void addStone(Point p) {
        modify();
        stones_->insert(p);
    }
    void removeStone(Point p) {
        modify();
        stones_->erase(p);
    }
    void clear() {
        modify();
        stones_->clear();
    }
    bool operator[](Point p) const {
        return stones_->count(p) != 0;
    }
    size_t size() const { return stones_->size(); }
    bool empty() const { return stones_->empty(); }
    const_iterator begin() const { return stones_->begin(); }
    const_iterator end() const { return stones_->end(); }

};

template <typename Archive>
void save(Archive& ar, const State& status, const unsigned int /*version*/)
{
    std::vector<Point> points;
    points.reserve(status.size());
    std::copy(status.begin(), status.end(), std::back_inserter(points));
    ar & points;
}

template <typename Archive>
void load(Archive& ar, State& state, const unsigned int /*version*/)
{
    std::vector<Point> points;
    ar & points;

    state = State{points};
}

inline bool isSubState(const State& subState, const State& state)
{
    return boost::find_if(subState,
            [&state](Point p) { return !state[p]; }) == subState.end();
}

inline bool operator!=(const State& lhs, const State& rhs)
{
    return !(lhs == rhs);
}

class AddStonesToState {
    State& state_;
public:
    AddStonesToState(State& state):
        state_(state){}
    AddStonesToState& operator()(Point  p)
    {
        state_.addStone(p);
        return *this;
    }
    State& get() { return state_; }
};

} // namespace sokoban

BOOST_SERIALIZATION_SPLIT_FREE(sokoban::State)

namespace std {

template<>
struct hash<sokoban::State> {
    size_t operator()(const sokoban::State& state) const
    {
        size_t result = 0;
        std::hash<sokoban::Point> h;
        for (sokoban::State::const_iterator it = state.begin();
                it != state.end(); ++it) {
            result += h(*it); // the order of elements doesn't count
        }
        return std::hash<size_t>()(result);
    }
};

} // namespace std

#endif /* STATE_H_ */
