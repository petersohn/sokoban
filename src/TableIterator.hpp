#ifndef TABLEITERATOR_H_
#define TABLEITERATOR_H_

#include "Mutexes.hpp"
#include "TimeMeter.hpp"
#include "ComplexChecker.hpp"
#include "Status/PointRange.hpp"
#include "util/ExplicitType.hpp"
#include <boost/optional.hpp>
#include <functional>

class State;
class Status;
class HeurCalculator;
class Table;

namespace boost { namespace asio { class io_service; }}

class TableIterator {
public:
	typedef std::function<void(const Status&)> Action;
private:
	const Table* table_;
	std::shared_ptr<const HeurCalculator> heurCalculator_;
	ComplexChecker checker_;
	Action action_;
	std::size_t minDistance_;
	std::size_t maxDistance_;
	std::size_t chokePointDistantNum_;
	std::size_t iters_, solved_;
	std::size_t lastTicks_;
	std::size_t workQueueLength_;
	std::size_t reverseSearchMaxDepth_;
	PointRange range_;
	Array<bool> excludeList_;
	Array<bool> chokePoints_;
	std::vector<Status> workQueue_;
	TimeMeter timeMeter_;
	enum class IterationState { idle, filling, working, done } iterationState_;

	MutexType iterMutex_;
	boost::asio::io_service& ioService_;
	ConditionVariableType done_;

	void initIter(PointRange::iterator it, std::size_t stones,
			const State& state);
	void doWork(const std::vector<Status>& statuses);
	void cleanWorkQueue();
	void progress();

public:

	using MinDistance = util::ExplicitType<struct tag_MinDistance, std::size_t>;
	using MaxDistance = util::ExplicitType<struct tag_MaxDistance, std::size_t>;
	using ChokePointDistantNum =
		util::ExplicitType<struct tag_ChokePointDistantNum, std::size_t>;
	using WorkQueueLength =
		util::ExplicitType<struct tag_WorkQueueLength, std::size_t>;
	using ReverseSearchMaxDepth =
		util::ExplicitType<struct tag_ReverseSearchMaxDepth, std::size_t>;

	TableIterator(
			const Table& table,
			const Action& action,
			MinDistance minDistance,
			MaxDistance maxDistance,
			ChokePointDistantNum chokePointDistantNum,
			Array<bool> chokePoints,
			WorkQueueLength workQueueLength,
			ReverseSearchMaxDepth reverseSearchMaxDepth,
			boost::asio::io_service& ioService);

	void start(std::size_t numStones,
			std::shared_ptr<const HeurCalculator> heurCalculator,
			ComplexChecker checker, boost::optional<Array<bool>> excludeList = {});
	void wait(bool print);
};


#endif /* TABLEITERATOR_H_ */
