/*
 * State.cpp
 *
 *  Created on: 1 Feb 2011
 *      Author: eszabpt
 */

#include "State.h"

using namespace std;

void State::hash()
{
	hashCode = 0;
	for (std::vector<Point>::iterator it = stones_.begin(); it != stones_.end(); ++it) {
		hashCode += it->x * it->y;
	}
}

void State::copyFrom(const State &other) {
	stones_ = other.stones();
	heur_ = other.heur();
	hashCode_ = other.hashCode();
}

bool State::operator==(const State &other) const
{
	if (other.stones().size() != stones().size())
		return false;
	for (size_t i = 0; i < stones().size(); i++)
	{
		bool van = false;
		for (size_t j = 0; j < stones().size(); j++)
		{
			if (other.stones[j]==stones[i])
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
	int result = heur() - other.heur();
	if (result == 0)
	{
		result = hashCode() - other.hashCode();
	}
	return result < 0;
}

void State::moveStone(int stone, const Point &p)
{
	stones[stone] = p;
	hash();
}


