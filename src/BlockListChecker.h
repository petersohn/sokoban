#ifndef BLOCKLISTCHECKER_H
#define BLOCKLISTCHECKER_H

#include "Checker.h"
#include "Solver.h"
#include "TableHeurCalculator.h"
#include "IndexedStatusList.h"
#include "DumperFunctions.h"
#include "ThreadPool.h"
#include <fstream>
#include <memory>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/date_time.hpp>

class BlockListChecker: public Checker, public TableHeurCalculator, public std::enable_shared_from_this<BlockListChecker> {
	Solver::Ptr solver_;
	HeurCalculator::Ptr calculator_;
	Checker::Ptr checker_;
	IndexedStatusList<int> blockList_;
	IndexedStatusList<int> heurList_;
	FixedTable::Ptr table_;
	int numStones_;
	int maxDistance_;
	std::ofstream dump_;
	MutexType dumpMutex_;

	std::deque<Node::Ptr> doCalculateBlockList(const Status& status);
	void calculateBlockList(const Status& status);
	void calculateHeurList(const Status& status);
	void dumpStatus(const Status &status, const Point *p, const std::string &title) {
		boost::lock_guard<MutexType> lck(dumpMutex_);
		if (!dump_.good())
			return;
		Array<bool> hl = status.reachableArray();
		if (p)
			hl[*p] = true;
		::dumpStatus(dump_, status, title, &hl);
	}
	virtual void init();
public:
	BlockListChecker(Solver::Ptr solver,
			HeurCalculator::Ptr calculator, Checker::Ptr checker, int numStones, int maxDistance);
	virtual bool check(const Status& status, const Point& p);
	virtual const char* errorMessage();
};

#endif /* BLOCKLISTCHECKER_H */
