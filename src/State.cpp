#include "State.h"
#include "Node.h"

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

bool State::operator<(const State &other) const
{
	int result = hashCode_ - other.hashCode_;
	return result < 0;
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

int State::Hash::operator()(const State &state)
{
	int result = 0;
	for (int i = 0; i < state.size(); ++i) {
		result += 47 * state[i].x + 61 * state[i].y;
	}
	return result;
}




