#ifndef TABLEITERATOR_H_
#define TABLEITERATOR_H_

#include "Mutexes.hpp"
#include "TimeMeter.hpp"
#include "ComplexChecker.hpp"
#include "HeurCalculator.hpp"
#include <boost/asio/io_service.hpp>
#include <functional>

class State;
class Status;

class TableIterator {
public:
	typedef std::function<void(const Status&)> Action;
private:
	const Table* table_;
	std::shared_ptr<const HeurCalculator> heurCalculator_;
	ComplexChecker checker_;
	Action action_;
	std::size_t maxDistance_;
	std::size_t iters_, solved_;
	std::size_t lastTicks_;
	std::size_t workQueueLength_;
	std::size_t reverseSearchMaxDepth_;
	std::vector<Status> workQueue_;
	TimeMeter timeMeter_;
	enum class IterationState { idle, filling, working, done } iterationState_;

	MutexType iterMutex_;
	boost::asio::io_service& ioService_;
	ConditionVariableType done_;

	void initIter(Point p, std::size_t stones, const State &state);
	void doWork(const std::vector<Status>& statuses);
	void cleanWorkQueue();
	void progress();

	bool advancePoint(Point& p);

public:
	TableIterator(
			const Table& table,
			const Action& action,
			std::size_t maxDistance,
			std::size_t workQueueLength,
			std::size_t reverseSearchMaxDepth,
			boost::asio::io_service& ioService):
		table_(&table),
		action_(action),
		maxDistance_(maxDistance),
		iters_(0),
		solved_(0),
		lastTicks_(-1),
		workQueueLength_(workQueueLength),
		reverseSearchMaxDepth_(reverseSearchMaxDepth),
		iterationState_(IterationState::idle),
		MUTEX_DECL(iterMutex_),
		ioService_(ioService)
	{
	}

	void start(std::size_t numStones,
			std::shared_ptr<const HeurCalculator> heurCalculator,
			ComplexChecker checker);
	void wait(bool print);
};


#endif /* TABLEITERATOR_H_ */
