#include "TableIterator.hpp"
#include "ProgressBar.hpp"
#include "Status/StatusUtils.hpp"
#include "Status/Table.hpp"
#include "Status/State.hpp"
#include "HeurCalculator.hpp"

bool TableIterator::advancePoint(Point& p)
{
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
		auto parts = getPartitions(*table_, state, reverseSearchMaxDepth_);
		bool ok = false;
		for (auto& status: parts) {
			float heur = heurCalculator_->calculateStatus(status);
			if (heur < 0) {
				continue;
			}
			if (!checkStatus(checker_, status)) {
				continue;
			}
			if (stones == 0) {
				// This will be accessed from another thread. Make sure that no
				// shared pointers that are potentially modified are shared
				// between the threads.
				workQueue_.push_back(status.deepCopy());
				if (workQueue_.size() == workQueueLength_) {
					cleanWorkQueue();
				}
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
		if (!table_->wall(p) && !state[p] && p != table_->destination()) {
			State state2(state);
			state2.addStone(p);
			initIter(p, stones - 1, state2);
		}
	} while (advancePoint(p));
}

void TableIterator::cleanWorkQueue() {
	++iters_;
	ioService_.post(std::bind(
			&TableIterator::doWork, this, std::move(workQueue_)));
	workQueue_.clear(); // in case it wouldn't be moved
}

void TableIterator::doWork(const std::vector<Status>& statuses)
{
	for (const auto& status: statuses) {
		action_(status);
	}
	boost::lock_guard<MutexType> lck(iterMutex_);
	++solved_;

	if (iterationState_ == IterationState::working && solved_ == iters_) {
		iterationState_ = IterationState::done;
	}

	done_.notify_all();
}

void TableIterator::start(std::size_t numStones,
			std::shared_ptr<const HeurCalculator> heurCalculator,
			ComplexChecker checker)
{
	assert(iterationState_ == IterationState::idle);
	heurCalculator_ = std::move(heurCalculator);
	checker_ = std::move(checker);
	solved_ = iters_ = 0;
	lastTicks_ = -1;
	iterationState_ = IterationState::filling;
	initIter(Point(0, 0), numStones, State());
	cleanWorkQueue();

	{
		boost::lock_guard<MutexType> lck(iterMutex_);

		if (solved_ == iters_) {
			iterationState_ = IterationState::done;
		} else {
			iterationState_ = IterationState::working;
		}
	}
}

void TableIterator::wait(bool print)
{
	if (iterationState_ == IterationState::idle) {
		return;
	}

	{
		std::unique_ptr<ProgressBar> progressBar;

		boost::unique_lock<MutexType> lock(iterMutex_);
		while (iterationState_ != IterationState::done) {
			done_.wait(lock);
			lock.unlock();

			if (print && !progressBar &&
					iterationState_ == IterationState::working) {
				progressBar.reset(new ProgressBar{iters_});
			}

			progressBar->draw(solved_);
			lock.lock();
		}
	}

	iterationState_ = IterationState::idle;

	if (print) {
		std::cerr << "Iteration processor time: " <<
				timeMeter_.processorTime() <<
				"\nIteration time: " <<
				timeMeter_.realTime() << std::endl;
	}
}

