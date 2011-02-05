/*
 * BasicHeurCalculator.cpp
 *
 *  Created on: 2011.02.04.
 *      Author: peet
 */

#include "BasicHeurCalculator.h"

virtual int BasicHeurCalculator::init() {
	distances_.reset(width, height, 0);
	distances_[problem().destination()] = 1;
	bool touched;
	Point p;
	do {
		touched = false;
		for (p.y = 0; p.y < height; p.y++)
			for (p.x = 0; p.x < width; p.x++)
			{
				if (distances_[p] == 0)
					continue;
				int newDist = distances[p]+1;
				if (checkDistance(p, p10, newDist))
					touched = true;
				if (checkDistance(p, pm10, newDist))
					touched = true;
				if (checkDistance(p, p01, newDist))
					touched = true;
				if (checkDistance(p, p0m1, newDist))
					touched = true;
			}
	} while (touched);
	for (p.y = 0; p.y < height; p.y++)
		for (p.x = 0; p.x < width; p.x++)
			distances[p]--;
}

bool BasicHeurCalculator::checkDistance(const Point & p, const Point & d, int dist)
{
	Point pd = p+d;
	bool result =
			problem().bareTable(pd) != ftWall &&
			problem().bareTable(p+d*2) != ftWall &&
			(distances_[pd] == 0 || distances_[pd] > dist);
	if (result)
		distances_[pd] = dist;
	return result;
}
