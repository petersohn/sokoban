#include "State.h"
#include "Node.h"
#include <boost/functional/hash.hpp>

using namespace std;

bool State::operator==(const State &other) const
{
	if (other.stones_.size() != stones_.size())
		return false;
	for (size_t i = 0; i < stones_.size(); i++)
	{
		bool van = false;
		for (size_t j = 0; j < stones_.size(); j++)
		{
			if (other.stones_[j]==stones_[i])
			{
				van = true;
				break;
			}
		}
		if (!van)
			return false;
	}
	return true;
}

int State::addStone(const Point & p)
{
	stones_.push_back(p);
	return stones_.size() - 1;
}

void State::removeStone(int stone)
{
	stones_.erase(stones_.begin()+stone);
}

void State::moveStone(int stone, const Point &p)
{
	stones_[stone] = p;
}



size_t hash_value(const State &state)
{
	size_t result = 0;
	boost::hash<Point> h;
	for (int i = 0; i < state.size(); ++i) {
		result += h(state[i]); // the order of elements doesn't count
	}
	return boost::hash<size_t>()(result);
}




