#ifndef TABLEITERATOR_H_
#define TABLEITERATOR_H_

#include "Common.h"
#include "Table.h"
#include "Status.h"
#include "Checker.h"
#include "Mutexes.hpp"
#include "ThreadPool.h"
#include <functional>

class TableIterator {
public:

	typedef std::function<void(const Status&)> Action;
private:
	FixedTable::Ptr table_;
	HeurCalculator::Ptr heurCalculator_;
	Checker::Ptr checker_;
	Action action_;
	int maxDistance_;
	int iters_, solved_;
	int lastTicks_;
	MutexType iterMutex_;
	boost::asio::io_service &ioService_;
	boost::condition_variable done_;

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
public:
	TableIterator(
			const FixedTable::Ptr& table,
			const HeurCalculator::Ptr& heurCalculator,
			const Checker::Ptr& checker,
			const Action& action,
			int maxDistance):
		table_(table),
		heurCalculator_(heurCalculator),
		checker_(checker),
		action_(action),
		maxDistance_(maxDistance),
		iters_(0),
		solved_(0),
		lastTicks_(-1),
		ioService_(ThreadPool::instance()->ioService())
	{
	}
	void iterate(int numStones);
};


#endif /* TABLEITERATOR_H_ */
