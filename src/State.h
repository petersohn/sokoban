#ifndef STATE_H_
#define STATE_H_

#include "Point.h"
#include <boost/unordered_set.hpp>
#include <assert.h>

class State {
public:
	typedef boost::unordered_set<Point> ContainerType;
	typedef ContainerType::const_iterator const_iterator;
	ContainerType stones_;
public:

	State() {}
	explicit State(const ContainerType &stones):stones_(stones) {
	}

	bool operator==(const State &other) const {
		return stones_ == other.stones_;
	}
	void addStone(const Point &p) {
		stones_.insert(p);
	}
	void removeStone(const Point &p) {
		stones_.erase(p);
	}
//	void setStones(const std::vector<Point> &stones) {
//		stones_=stones;
//	}
	bool operator[](const Point &p) const {
		return stones_.count(p) != 0;
	}
	size_t size() const { return stones_.size(); }
	const_iterator begin() const { return stones_.begin(); }
	const_iterator end() const { return stones_.end(); }
};

size_t hash_value(const State &state);

#endif /* STATE_H_ */
