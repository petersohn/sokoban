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
	const Table* table_;
	HeurCalculator::Ptr heurCalculator_;
	Checker::Ptr checker_;
	Action action_;
	std::size_t maxDistance_;
	std::size_t iters_, solved_;
	std::size_t lastTicks_;
	std::size_t workQueueLength_;
	std::vector<Status> workQueue_;
	bool working_;

	MutexType iterMutex_;
	boost::asio::io_service &ioService_;
	ConditionVariableType done_;

	void initIter(Point p, std::size_t stones, const State &state);
	void doWork(const std::vector<Status>& statuses);
	void cleanWorkQueue();
	void progress();
	bool advancePoint(Point &p) {
		if (p.x == static_cast<int>(table_->width()) - 1) {
			if (p.y < static_cast<int>(table_->height()) - 1) {
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
			const Table& table,
			const Action& action,
			std::size_t maxDistance,
			std::size_t workQueueLength,
			util::ThreadPool& threadPool):
		table_(&table),
		action_(action),
		maxDistance_(maxDistance),
		iters_(0),
		solved_(0),
		lastTicks_(-1),
		workQueueLength_(workQueueLength),
		working_(false),
		MUTEX_DECL(iterMutex_),
		ioService_(threadPool.getIoService())
	{
	}

	const Action& getAction() { return action_; }
	void setAction(const Action& action)
	{
		assert(!working_);
		action_ = action;
	}
	void iterate(std::size_t numStones,
			const HeurCalculator::Ptr& heurCalculator,
			const Checker::Ptr& checker);
};


#endif /* TABLEITERATOR_H_ */
