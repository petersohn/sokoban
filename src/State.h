#ifndef STATE_H_
#define STATE_H_

#include "Point.h"
#include <vector>

class State {
	std::vector<Point> stones_;
	int hashCode_;
	void hash();
	void copyFrom(const State &other);
public:

	State():hashCode_(0) {}
	explicit State(const std::vector<Point> stones) {
		setStones(stones);
	}
	State(const State &other) {
		copyFrom(other);
	}
	State &operator=(const State &other) {
		copyFrom(other);
		return *this;
	}

	bool operator==(const State &other) const;
	bool operator<(const State &other) const;
	void moveStone(int stone, const Point &p);
	int addStone(const Point &p);
	void removeStone(int stone);
	void setStones(const std::vector<Point> &stones) {
		stones_=stones;
		hash();
	}
	const Point& operator[](int stone) const {
		assert(stone >= 0 && stone < stones_.size());
		return stones_[stone];
	}
	size_t size() const { return stones_.size(); }
};

class Node;

class VisitedState: public State
{
private:
	Point currentPos_;
public:
	VisitedState() {}
	VisitedState(const VisitedState &other):State(other)
	{
		currentPos_ = other.currentPos();
	}
	explicit VisitedState(const State &other):State(other) {}
	explicit VisitedState(const Node &other);

	const Point& currentPos() const { return currentPos_; }
	void currentPos(const Point &p) { currentPos_ = p; }
};

#endif /* STATE_H_ */
