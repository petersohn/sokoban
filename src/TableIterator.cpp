#include "TableIterator.hpp"
#include "TempValue.hpp"
#include "ProgressBar.hpp"
#include "TimeMeter.hpp"
#include "Status/StatusUtils.hpp"

void TableIterator::initIter(Point p, std::size_t stones, const State &state)
{
	if (!state.empty()) {
		if (maxDistance_ > 0) {
			for (Point pp: state) {
				if (static_cast<std::size_t>(std::abs(p.x - pp.x)) > maxDistance_ ||
						static_cast<std::size_t>(std::abs(p.y - pp.y)) > maxDistance_) {
					return;
				}
			}
		}
		std::vector<Status::Ptr> parts = getPartitions(table_, state);
		bool ok = false;
		for (const Status::Ptr& status: parts) {
			int heur = heurCalculator_->calculateStatus(*status);
			if (heur < 0) {
				continue;
			}
			if (!checkStatus(*checker_, *status)) {
				continue;
			}
			if (stones == 0) {
				++iters_;
				ioService_.post(std::bind(&TableIterator::doWork, this, status));
			} else {
				ok = true;
			}
		}
		if (!ok) {
			return;
		}
		assert(stones > 0);
	}
	do {
		if (!table_->get().wall(p) && !state[p] && p != table_->get().destination()) {
			State state2(state);
			state2.addStone(p);
			initIter(p, stones - 1, state2);
		}
	} while (advancePoint(p));
}

void TableIterator::doWork(Status::Ptr status)
{
	action_(*status);
	boost::lock_guard<MutexType> lck(iterMutex_);
	++solved_;
	done_.notify_all();
}

void TableIterator::iterate(std::size_t numStones)
{
	assert(!working_);
	TempValue<bool> working(working_, true);
	TimeMeter timeMeter;
	solved_ = iters_ = 0;
	lastTicks_ = -1;
	initIter(Point(0, 0), numStones, State());
	{
		ProgressBar progressBar(iters_);
		boost::unique_lock<MutexType> lck(iterMutex_);
		while (solved_ < iters_) {
			done_.wait(lck);
			lck.unlock();
			progressBar.draw(solved_);
			lck.lock();
		}
	}
	std::cerr << "Iteration processor time: " <<
			timeMeter.processorTime() <<
			"\nIteration time: " <<
			timeMeter.realTime() << std::endl;
}
