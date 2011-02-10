#include "AdvancedHeurCalculator.h"
#include "Status.h"
#include "Solver.h"
#include "Node.h"
#include <vector>
#include <algorithm>

void AdvancedHeurCalculator::init()
{
	Array<bool> kell(width, height, false);
	partitions_.reset(width, height);
	Point p;
	for (p.y = 0; p.y < table().height(); p.y++)
		for (p.x = 0; p.x < table().width(); p.x++)
		{
			if (table().value(p) == ftWall)
				continue;
			int kellNum = 0;
			Point pp;
			for (pp.y = 0; pp.y < height; pp.y++)
				for (pp.x = 0; pp.x < width; pp.x++)
					if (table().value(p) == ftFloor)
					{
						kell[pp] = true;
						++kellNum;
					} else
						kell[pp] = false;
			while (kellNum > 0)
				initPartition(p, kell, kellNum);
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
		int &kellNum)
{
	Point pp;
	for (pp.y = 0; pp.y < height; pp.y++)
		for (pp.x = 0; pp.x < width; pp.x++)
			if (kell[pp])
				goto ki;
ki:
	VisitedState state;
	state.addStone(p);
	Status status(tablePtr(), state);
	Array<bool> reach(width, height, false);
	floodFill(status, pp, reach);
	Partition part;
	part.heur = -1;
	if (p == destination)
		part.heur = 0;
	else {
		Solver s(problem, calculator_);
		std::vector<Node::Ptr> res = s.solve();
		if (res.size() != 0)
			part.heur = (*res.rbegin())->cost();
	}
	part.reachable = reach;
	partitions_[p].push_back(part);
	for (pp.y = 0; pp.y < height; pp.y++)
		for (pp.x = 0; pp.x < width; pp.x++)
			if (reach[pp] && kell[pp])
			{
				kell[pp] = false;
				--kellNum;
			}
}

int AdvancedHeurCalculator::doCalculateStone(const Status &status, const Point &p) const
{
	std::vector<Partition>::iterator it;
	if (status.state().currentPos() == p) {
		int min = -1;
		for (it = partitions_[p].begin();
			it != partitions_[p].end(); ++it) {
			if (it->heur >= 0) {
				min = it->heur;
				break;
			}
		}
		while (++it != partitions_[p].end())
			if (it->heur < min)
				min = it->heur;
	} else {
		for (it = partitions_[p].begin();
			it != partitions_[p].end(); ++it)
		{
			if (it->reachable[status.state().currentPos()])
				break;
		}
		if (it != partitions[p].end())
			return it->heur;
		return -1;
	}
}



