#ifndef STATE_H_
#define STATE_H_

#include "Point.h"
#include <boost/unordered_set.hpp>
#include <boost/shared_ptr.hpp>
#include <assert.h>

class State {
	typedef boost::unordered_set<Point> ContainerType;
	typedef boost::shared_ptr<ContainerType> ContainerPtr;
	ContainerPtr stones_;
	void modify() {
		if (stones_.use_count() > 1)
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

size_t hash_value(const State &state);

#endif /* STATE_H_ */
