#include "AdvancedHeurCalculator.h"
#include "Status.h"
#include "Solver.h"
#include "Node.h"
#include <vector>
#include <algorithm>
#include <boost/format.hpp>
#include <string>


/* AdvancedHeurCalculator::HeurDumper */

void AdvancedHeurCalculator::HeurDumper::open() {
	if (!file_.is_open())
		file_.open("partitions.dump", std::ofstream::out | std::ofstream::trunc);
}

void AdvancedHeurCalculator::HeurDumper::dumpPartition(const AdvancedHeurCalculator &calc,
		const Partition & part) {
	open();
	file_ << "Partition (" << part.heur << ")" << std::endl;
	Status s(calc.tablePtr());
	s.addStone(part.pos);
	dumpStatus(file_, s, "", &part.reachable);
	file_ << std::endl;
}

void AdvancedHeurCalculator::HeurDumper::printText(const char *text) {
	open();
	file_ << text << std::endl;
}


/* AdvancedHeurCalculator */

AdvancedHeurCalculator::HeurDumper AdvancedHeurCalculator::dumper;

void AdvancedHeurCalculator::init()
{
	Array<bool> kell(table().width(), table().height(), false);
	Array<std::string> dump(table().width(), table().height());
	std::vector<Partition> dumpPartitions;
	partitions_.reset(table().width(), table().height());
	Point p;
	for (p.y = 0; p.y < table().height(); p.y++)
		for (p.x = 0; p.x < table().width(); p.x++)
		{
			if (table().wall(p))
				continue;
			int kellNum = 0;
			Point pp;
			for (pp.y = 0; pp.y < table().height(); pp.y++)
				for (pp.x = 0; pp.x < table().width(); pp.x++)
					if (!table().wall(p))
					{
						kell[pp] = true;
						++kellNum;
					} else
						kell[pp] = false;
			while (kellNum > 0)
				initPartition(p, kell, kellNum);
			dump[p] =
					partitions_[p].size() == 0 ? "*" :
					partitions_[p].size() > 1 ? "?" :
					(boost::format("%d") % partitions_[p][0].heur).str();
		}
	dumper.dumpArray(dump);
	for (p.y = 0; p.y < table().height(); p.y++)
		for (p.x = 0; p.x < table().width(); p.x++) {
			if (partitions_[p].size() > 1)
				for (std::vector<Partition>::iterator it = partitions_[p].begin();
						it != partitions_[p].end(); ++it)
					dumper.dumpPartition(*this, *it);
		}

}

void AdvancedHeurCalculator::initPartition(const Point & p, Array<bool> &kell,
		int &kellNum)
{
	Point pp;
	for (pp.y = 0; pp.y < table().height(); pp.y++)
		for (pp.x = 0; pp.x < table().width(); pp.x++)
			if (kell[pp])
				goto ki;
ki:
	VisitedState state;
	state.addStone(p);
	state.currentPos(pp);
	Status status(tablePtr(), state);
	Partition part(table().width(), table().height());
	part.pos = p;
	part.heur = -1;
	if (p == table().destination())
		part.heur = 0;
	else {
		Solver s;
		std::deque<Node> res = s.solve(status, calculator_);
		if (res.size() != 0)
			part.heur = res.rbegin()->cost();
	}
	partitions_[p].push_back(part);
	for (pp.y = 0; pp.y < table().height(); pp.y++)
		for (pp.x = 0; pp.x < table().width(); pp.x++) {
			part.reachable[pp] = status.reachable(pp);
			if (status.reachable(pp) && kell[pp])
			{
				kell[pp] = false;
				--kellNum;
			}
		}
	dumper.printText("---\n");
}

int AdvancedHeurCalculator::doCalculateStone(const Status &status, const Point &p)
{
	std::vector<Partition>::const_iterator it;
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
		return min;
	} else {
		for (it = partitions_[p].begin();
			it != partitions_[p].end(); ++it)
		{
			if (it->reachable[status.state().currentPos()])
				break;
		}
		if (it != partitions_[p].end())
			return it->heur;
	}
	return -1;
}



