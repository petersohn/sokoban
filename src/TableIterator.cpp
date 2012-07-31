#include "TableIterator.h"
#include "TempValue.h"
#include "ProgressBar.h"
#include <ctime>

void TableIterator::initIter(Point p, int stones, const State &state)
{
	if (state.size() > 0) {
		if (maxDistance_ > 0) {
			for (const Point &pp: state) {
				if (std::abs(p.x - pp.x) > maxDistance_ ||
						std::abs(p.y - pp.y) > maxDistance_) {
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
			bool ok2 = true;
			for (const Point &pp: state) {
				if (!checker_->check(*status, pp)) {
					ok2 = false;
					break;
				}
			}
			if (!ok2) {
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

void TableIterator::iterate(int numStones)
{
	assert(!working_);
	TempValue<bool> working(working_, true);
	std::clock_t iterationStart = std::clock();
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
	clock_t processorTime = clock() - iterationStart;
	std::cerr << "Iteration processor time: " <<
			(double)processorTime/CLOCKS_PER_SEC << std::endl;
}
