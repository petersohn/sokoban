/*
 * BasicHeurCalculator.cpp
 *
 *  Created on: 2011.02.04.
 *      Author: peet
 */

#include "BasicHeurCalculator.h"

virtual int BasicHeurCalculator::calculate(Problem &problem, Status &status) {
	Point p;
	distances[p0] = 1;
	bool touched;
	do {
		touched = false;
		for (p.y = 0; p.y < height; p.y++)
			for (p.x = 0; p.x < width; p.x++)
			{
				if (distances[p] == 0)
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
