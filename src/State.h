#ifndef STATE_H_
#define STATE_H_

#include "Point.h"
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
	State(const State &other):stones_(other.stones_) {}
//	explicit State(const ContainerType &stones):stones_(stones) {
//	}

	State& operator=(const State &other) {
		stones_ = other.stones_;
		return *this;
	}

	bool operator==(const State &other) const {
		return stones_ == other.stones_ || *stones_ == *(other.stones_);
	}
	void addStone(const Point &p) {
		modify();
		stones_->insert(p);
	}
	void removeStone(const Point &p) {
		modify();
		stones_->erase(p);
	}
//	void setStones(const std::vector<Point> &stones)0 {
//		stones_=stones;
//	}
	bool operator[](const Point &p) const {
		return stones_->count(p) != 0;
	}
	size_t size() const { return stones_->size(); }
	const_iterator begin() const { return stones_->begin(); }
	const_iterator end() const { return stones_->end(); }
};

inline bool isSubState(const State& subState, const State& state)
{
	return boost::find_if(subState,
			[&state](const Point &p) { return !state[p]; }) == subState.end();
}

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
