#include "TableIterator.h"

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

void TableIterator::progress()
{
	while (true) {
		int iters, solved;
		{
			boost::lock_guard<MutexType> lck(iterMutex_);
			iters = iters_;
			solved = solved_;
		}
		if (solved < iters) {
			float div = iters / 100.0f;
			int n1 = (int)(solved / div);
			int n2 = 100 - n1;
			std::cerr << "[";
			for (int i = 0; i < n1; ++i)
				std::cerr << "#";
			for (int i = 0; i < n2; ++i)
				std::cerr << "-";
			std::cerr << "]\r";
			std::cerr.flush();
			try {
				boost::this_thread::sleep(progressInterval_);
			} catch(boost::thread_interrupted &e) {
				return;
			}
		}
	}
}

void TableIterator::iterate(int numStones)
{
	solved_ = iters_ = 0;
	initIter(Point(0, 0), numStones, State());
	std::unique_ptr<boost::thread> progressThread;
	if (!progressInterval_.is_not_a_date_time()) {
		std::cerr << std::endl;
		progressThread.reset(new boost::thread(std::bind(&TableIterator::progress, this)));
	}
	{
		boost::unique_lock<MutexType> lck(iterMutex_);
		while (solved_ < iters_) {
			done_.wait(lck);
		}
	}
	if (progressThread) {
		progressThread->interrupt();
		progressThread->join();
		std::cerr << std::endl;
	}
}
