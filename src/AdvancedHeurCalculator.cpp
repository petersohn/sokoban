#include "AdvancedHeurCalculator.h"
#include "Status.h"
#include "Solver.h"

void AdvancedHeurCalculator::init()
{
	Array<bool> kell(width, height, false);
	partitions_.reset(width, height);
	Point p;
	Problem simpleProblem(table());
	for (p.y = 0; p.y < table().height(); p.y++)
		for (p.x = 0; p.x < table().width(); p.x++)
		{
			if (simpleProblem.tableValue(p) == ftWall)
				continue;
			simpleProblem.addStone(p);
			int kellNum = 0;
			Point pp;
			for (pp.y = 0; pp.y < height; pp.y++)
				for (pp.x = 0; pp.x < width; pp.x++)
					if (simpleProblem.tableValue(p) == ftFloor)
					{
						kell[pp] = true;
						++kellNum;
					} else
						kell[pp] = false;
			while (kellNum > 0)
				initPartition(p, kell, kellNum, simpleProblem);
			simpleProblem.removeStone(p);
		}
	goodTiles.reset(width, height, false);
	for (p.y = 0; p.y < height; p.y++)
		for (p.x = 0; p.x < width; p.x++) {
			for (vector<Partition>::iterator it = partitions[p].begin();
					it != partitions[p].end(); ++it) {
				if (it->heur >= 0) {
					goodTiles[p] = true;
					break;
				}
			}
		}
}

void AdvancedHeurCalculator::initPartition(const Point & p, Array<bool> &kell,
		int &kellNum, Problem &problem)
{
	Point pp;
	for (pp.y = 0; pp.y < height; pp.y++)
		for (pp.x = 0; pp.x < width; pp.x++)
			if (kell[pp])
				goto ki;
ki:
	Array<bool> reach(width, height, false);
	floodFill(pp, reach);
	Partition part;
	part.heur = -1;
	if (p == destination)
		part.heur = 0;
	else
		solveIt();
	part.reachable = reach;
	if (part.heur < 0 && state.get() != NULL)
		part.heur = state->cost;
	partitions[p].push_back(part);
	for (pp.y = 0; pp.y < height; pp.y++)
		for (pp.x = 0; pp.x < width; pp.x++)
			if (reach[pp] && kell[pp])
			{
				kell[pp] = false;
				kellNum--;
			}
}

int AdvancedHeurCalculator::doCalculate(const Status & status)
{
}



