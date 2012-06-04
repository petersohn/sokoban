#include "AdvancedHeurCalculator.h"
#include "Status.h"
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
	Status s(calc.tablePtr());
	s.addStone(part.pos);
	s.currentPos(Point(-1, -1));
	dumpStatus(file_, s, (boost::format("Partition (%d)") % part.heur).str(), &part.reachable);
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
	Array<std::string> dump(table().width(), table().height());
	std::vector<Partition> dumpPartitions;
	partitions_.reset(table().width(), table().height());
	Point p;
	for (p.y = 0; p.y < table().height(); p.y++)
		for (p.x = 0; p.x < table().width(); p.x++)
		{
			if (table().wall(p)) {
				dump[p] = "*";
				continue;
			}
			initPartitions(p);
			dump[p] =
					partitions_[p].size() == 0 ? "#" :
					partitions_[p].size() > 1 ? "?" :
					(boost::format("%d") % partitions_[p][0].heur).str();
		}
	dumper.printText("Heuristics table:");
	dumper.dumpArray(dump);
	dumper.printText("\nPartitions:");
	for (p.y = 0; p.y < table().height(); p.y++)
		for (p.x = 0; p.x < table().width(); p.x++) {
			if (partitions_[p].size() > 1)
				for (std::vector<Partition>::iterator it = partitions_[p].begin();
						it != partitions_[p].end(); ++it)
					dumper.dumpPartition(*this, *it);
		}

}

void AdvancedHeurCalculator::initPartitions(const Point & p)
{
	State state;
	state.addStone(p);
	std::vector<Status::Ptr> parts = getPartitions(tablePtr(), state);
	for (Status::Ptr status: parts) {
		Partition part(table().width(), table().height());
		part.pos = p;
		part.heur = -1;
		part.reachable = status->reachableArray();
		if (p == table().destination())
			part.heur = 0;
		else {
			std::deque<Node::Ptr> res = solver_->solve(*status);
			if (res.size() != 0)
				part.heur = (*res.rbegin())->cost();
		}
		partitions_[p].push_back(part);
	}
}

int AdvancedHeurCalculator::doCalculateStone(const Status &status, const Point &p)
{
	std::vector<Partition>::const_iterator it;
	// If the current position equals p, then partitions
	// can't be used. Use the minimal non-negative
	// partition's value instead
	if (status.currentPos() == p) {
		int min = -1;
		for (it = partitions_[p].begin();
			it != partitions_[p].end(); ++it) {
			if (it->heur >= 0) {
				min = it->heur;
				break;
			}
		}
		if (it != partitions_[p].end())
			while (++it != partitions_[p].end())
				if (it->heur >= 0 && it->heur < min)
					min = it->heur;
		return min;
	} else {
		for (it = partitions_[p].begin();
			it != partitions_[p].end(); ++it)
		{
			if (it->reachable[status.currentPos()])
				break;
		}
		if (it != partitions_[p].end())
			return it->heur;
	}
	return -1;
}



