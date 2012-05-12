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

class BlockListChecker: public Checker {
	typedef boost::mutex MutexType;

	Solver::Ptr solver_;
	HeurCalculator::Ptr calculator_;
	Checker::Ptr checker_;
	IndexedStatusList blockList_;
	FixedTable::Ptr table_;
	int numStones_;
	int maxDistance_;
	int iters_, solved_;
	std::ofstream dump_;
	MutexType iterMutex_, listMutex_, dumpMutex_;
	boost::asio::io_service &ioService_;
	boost::condition_variable done_;
	boost::posix_time::time_duration progressInterval_;

	void initIter(Point p, int stones, const State &state);
	void doWork(Status::Ptr status);
	void progress();
	bool advancePoint(Point &p) {
		if (p.x == table_->get().width() - 1) {
			if (p.y < table_->get().height() - 1) {
				++p.y;
				p.x = 0;
			} else {
				return false;
			}
		} else {
			++p.x;
		}
		return true;
	}
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
			HeurCalculator::Ptr calculator, Checker::Ptr checker, int numStones, int maxDistance,
			int progressInterval);
	void init(FixedTable::Ptr table);
	virtual bool check(const Status& status, const Point& p);
	virtual const char* errorMessage();
};

#endif /* BLOCKLISTCHECKER_H */
