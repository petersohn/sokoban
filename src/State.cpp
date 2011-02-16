#include "State.h"
#include "Node.h"

using namespace std;

void State::hash()
{
	hashCode_ = 0;
	for (std::vector<Point>::iterator it = stones_.begin(); it != stones_.end(); ++it) {
		hashCode_ += it->x * it->y;
	}
}

void State::copyFrom(const State &other) {
	stones_ = other.stones_;
	hashCode_ = other.hashCode_;
}

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
	hash();
	return stones_.size() - 1;
}

void State::removeStone(int stone)
{
	stones_.erase(stones_.begin()+stone);
	hash();
}

void State::moveStone(int stone, const Point &p)
{
	stones_[stone] = p;
	hash();
}

VisitedState::VisitedState(const Node &other):
		State(other.state()),
		currentPos_(operator[](other.stone()) - other.d())
{
}





