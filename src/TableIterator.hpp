#ifndef TABLEITERATOR_H_
#define TABLEITERATOR_H_

#include "Common.hpp"
#include "Status/Table.hpp"
#include "Status/Status.hpp"
#include "Checker.hpp"
#include "Mutexes.hpp"
#include "util/ThreadPool.hpp"
#include <functional>


class TableIterator {
public:
	typedef std::function<void(const Status&)> Action;
private:
	FixedTable::Ptr table_;
	HeurCalculator::Ptr heurCalculator_;
	Checker::Ptr checker_;
	Action action_;
	std::size_t maxDistance_;
	std::size_t iters_, solved_;
	std::size_t lastTicks_;
	std::vector<Status::Ptr> workQueue;
	bool working_;

	MutexType iterMutex_;
	boost::asio::io_service &ioService_;
	boost::condition_variable done_;

	void initIter(Point p, std::size_t stones, const State &state);
	void doWork(std::vector<Status::Ptr> statuses);
	void cleanWorkQueue();
	void progress();
	bool advancePoint(Point &p) {
		if (p.x == static_cast<int>(table_->get().width()) - 1) {
			if (p.y < static_cast<int>(table_->get().height()) - 1) {
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
			std::size_t maxDistance,
			util::ThreadPool& threadPool):
		table_(table),
		heurCalculator_(heurCalculator),
		checker_(checker),
		action_(action),
		maxDistance_(maxDistance),
		iters_(0),
		solved_(0),
		lastTicks_(-1),
		working_(false),
		ioService_(threadPool.getIoService())
	{
	}

	const Action& getAction() { return action_; }
	void setAction(const Action& action)
	{
		assert(!working_);
		action_ = action;
	}
	void iterate(std::size_t numStones);
};


#endif /* TABLEITERATOR_H_ */
