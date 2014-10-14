#ifndef STATE_H_
#define STATE_H_

#include "Status/Point.hpp"
#include <unordered_set>
#include <memory>
#include <assert.h>
#include <functional>
#include <boost/range/algorithm.hpp>

class State {
	typedef std::unordered_set<Point> ContainerType;
	typedef std::shared_ptr<ContainerType> ContainerPtr;
	ContainerPtr stones_;
	void modify() {
		if (!stones_.unique())
			stones_.reset(new ContainerType(*stones_));
	}
public:
	typedef ContainerType::const_iterator const_iterator;

	State():stones_(new ContainerType) {}
	State(const State& other) = default;
	State& operator=(const State& other) = default;
	State(State&& other) = default;
	State& operator=(State&& other) = default;

	State deepCopy() const {
		State result{*this};
		result.modify();
		return result;
	}

	bool operator==(const State &other) const {
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



namespace std {

template<>
struct hash<State> {
	size_t operator()(const State &state) const
	{
		size_t result = 0;
		std::hash<Point> h;
		for (State::const_iterator it = state.begin();
				it != state.end(); ++it) {
			result += h(*it); // the order of elements doesn't count
		}
		return std::hash<size_t>()(result);
	}
};

}


#endif /* STATE_H_ */
