#ifndef BLOCKLISTGENERATOR_H
#define BLOCKLISTGENERATOR_H

#include "Solver.h"
#include "IndexedStatusList.h"
#include "DumperFunctions.h"
#include "ThreadPool.h"
#include "Checker.h"
#include "HeurInfo.h"
#include <fstream>
#include <memory>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/date_time.hpp>

class BlockListGenerator {
	Solver::Ptr solver_;
	HeurCalculator::Ptr calculator_;
	Checker::Ptr checker_;
	std::shared_ptr<IndexedStatusList<int>> blockList_;
	std::shared_ptr<std::vector<HeurInfo>> heurList_;
	FixedTable::Ptr table_;
	int numStones_;
	int maxDistance_;
	int maxHeurListSize_;
	std::ofstream dump_;
	MutexType dumpMutex_;

	std::deque<Node::Ptr> doCalculateBlockList(const Status& status);
	void calculateBlockList(const Status& status);
	void calculateHeurList(const Status& status);
	void dumpStatus(const Status &status, const Point *p, const std::string &title) {
		boost::lock_guard<MutexType> lck(dumpMutex_);
		if (!dump_.good())
			return;
		if (p) {
			Array<bool> hl = status.reachableArray();
			hl[*p] = true;
			::dumpStatus(dump_, status, title, &hl);
		} else {
			::dumpStatus(dump_, status, title, &status.reachableArray());
		}
	}
public:
	BlockListGenerator(Solver::Ptr solver,
			HeurCalculator::Ptr calculator, Checker::Ptr checker, int numStones,
			int maxDistance, int maxHeurListSize);
	Checker::Ptr checker();
	HeurCalculator::Ptr heurCalculator();
	void init(const FixedTable::Ptr& table);
};

#endif /* BLOCKLISTGENERATOR_H */
