#ifndef STATE_H_
#define STATE_H_

#include "Point.h"
#include <vector>
#include <assert.h>

class State {
	std::vector<Point> stones_;
public:

	State() {}
	explicit State(const std::vector<Point> stones) {
		setStones(stones);
	}

	bool operator==(const State &other) const;
	void moveStone(int stone, const Point &p);
	int addStone(const Point &p);
	void removeStone(int stone);
	void setStones(const std::vector<Point> &stones) {
		stones_=stones;
	}
	const Point& operator[](int stone) const {
		assert(stone >= 0 && stone < stones_.size());
		return stones_[stone];
	}
	size_t size() const { return stones_.size(); }

	struct Hash {
		int operator()(const State &state);
	};
};

#endif /* STATE_H_ */
