#include "AdvancedHeurCalculator.hpp"
#include "Status/Status.hpp"
#include "Status/StatusUtils.hpp"
#include "Node.hpp"
#include <vector>
#include <algorithm>
#include <boost/format.hpp>
#include <string>
#include <algorithm>


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
	for (Point  p: arrayRange(table())) {
		if (table().wall(p)) {
			dump[p] = "*";
			continue;
		}
		initPartitions(p);
		dump[p] =
				partitions_[p].empty() ? "#" :
				partitions_[p].size() > 1 ? "?" :
				(boost::format("%d") % partitions_[p][0].heur).str();
	}
	if (useDumper_) {
		dumper.printText("Heuristics table:");
		dumper.dumpArray(dump);
		dumper.printText("\nPartitions:");
		for (Point  p: arrayRange(table())) {
			if (partitions_[p].size() > 1) {
				for (const Partition& partition: partitions_[p]) {
					dumper.dumpPartition(*this, partition);
				}
			}
		}
	}
}

void AdvancedHeurCalculator::initPartitions(Point  p)
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
			if (!res.empty())
				part.heur = res.back()->cost();
		}
		partitions_[p].push_back(part);
	}
}

int AdvancedHeurCalculator::doCalculateStone(const Status &status, Point p)
{
	std::vector<Partition>::const_iterator it;
	// If the current position equals p, then partitions
	// can't be used. Use the minimal non-negative
	// partition's value instead
	if (status.currentPos() == p) {
		auto firstOk = std::find_if(partitions_[p].begin(), partitions_[p].end(),
				[](const Partition& partition) { return partition.heur >= 0; });
		auto minElement = std::min_element(firstOk, partitions_[p].end(),
				[](const Partition& left, const Partition& right)
				{ return left.heur < right.heur; });
		return (minElement == partitions_[p].end()) ? -1 : minElement->heur;
	} else {
		auto it = std::find_if(partitions_[p].begin(), partitions_[p].end(),
				[&status](const Partition& partition)
				{ return partition.reachable[status.currentPos()]; });
		return (it == partitions_[p].end()) ? -1 : it->heur;
	}
}



