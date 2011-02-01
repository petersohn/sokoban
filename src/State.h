#ifndef STATE_H_
#define STATE_H_

#include "Point.h"
#include <vector>

class State {
	std::vector<Point> stones_;
	int hashCode_;
	int heur_;
	void hash();
	void copyFrom(const State &other);
public:
	State():hashCode_(0), heur_(0) {}
	State(const std::vector<Point> stones) {
		stones_ = stones;
		hash();
	}
	State(const State &other) {
		copyFrom(other);
	}
	State &operator=(const State &other) {
		copyFrom(other);
		return *this;
	}
	State& operator=(const std::vector<Point> &stones) {
		stones_=stones;
		hash();
		return *this;
	}

	int heur() const { return heur_; }

	bool operator==(const State &other) const;
	bool operator<(const State &other) const;
	void moveStone(int stone, const Point &p);
	const Point& operator[](int stone) const { return stones[stone]; }
	size_t size() const { return stones.size(); }
};

#endif /* STATE_H_ */
