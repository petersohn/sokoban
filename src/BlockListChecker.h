#ifndef BLOCKLISTCHECKER_H
#define BLOCKLISTCHECKER_H

#include "Checker.h"
#include "Solver.h"
#include "HeurCalculator.h"
#include "IndexedStatusList.h"
#include "DumperFunctions.h"
#include "ThreadPool.h"
#include <fstream>
#include <memory>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/date_time.hpp>

class BlockListChecker: public Checker, public std::enable_shared_from_this<BlockListChecker> {
	Solver::Ptr solver_;
	HeurCalculator::Ptr calculator_;
	Checker::Ptr checker_;
	IndexedStatusList<int> blockList_;
	FixedTable::Ptr table_;
	int numStones_;
	int maxDistance_;
	std::ofstream dump_;
	MutexType dumpMutex_;

	void doWork(const Status& status);
	void dumpStatus(const Status &status, const Point *p, const std::string &title) {
		boost::lock_guard<MutexType> lck(dumpMutex_);
		if (!dump_.good())
			return;
		Array<bool> hl = status.reachableArray();
		if (p)
			hl[*p] = true;
		::dumpStatus(dump_, status, title, &hl);
	}
public:
	BlockListChecker(Solver::Ptr solver,
			HeurCalculator::Ptr calculator, Checker::Ptr checker, int numStones, int maxDistance);
	void init(FixedTable::Ptr table);
	virtual bool check(const Status& status, const Point& p);
	virtual const char* errorMessage();
};

#endif /* BLOCKLISTCHECKER_H */
