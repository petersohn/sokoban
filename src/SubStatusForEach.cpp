#include "SubStatusForEach.hpp"
#include "ProgressBar.hpp"
#include "Status/StatusUtils.hpp"
#include "Status/Table.hpp"
#include "Status/State.hpp"
#include "HeurCalculator.hpp"
#include "ComplexCheckerBase.hpp"
#include <boost/asio/io_service.hpp>

SubStatusForEach::SubStatusForEach(
            const Table& table,
            const Action& action,
            MinDistance minDistance,
            MaxDistance maxDistance,
            ChokePointDistantNum chokePointDistantNum,
            Array<bool> chokePoints,
            WorkQueueLength workQueueLength,
            ReverseSearchMaxDepth reverseSearchMaxDepth,
            boost::asio::io_service& ioService):
        table_(&table),
        action_(action),
        minDistance_(minDistance.value()),
        maxDistance_(maxDistance.value()),
        chokePointDistantNum_(chokePointDistantNum.value()),
        iters_(0),
        solved_(0),
        lastTicks_(-1),
        workQueueLength_(workQueueLength.value()),
        reverseSearchMaxDepth_(reverseSearchMaxDepth.value()),
        range_(arrayRange(table)),
        chokePoints_(std::move(chokePoints)),
        iterationState_(IterationState::idle),
        MUTEX_DECL(iterMutex_),
        ioService_(ioService)
    {
    }

void SubStatusForEach::initIter(PointRange::iterator it, std::size_t stones,
        const State& state)
{
    if (!state.empty()) {
        Point p = *it;
        if (maxDistance_ > 0 || minDistance_ > 0) {
            std::size_t numWrongDistance = 0;
            for (Point pp: state) {
                if (pp == p) {
                    continue;
                }
                std::size_t xDistance = std::abs(p.x - pp.x);
                std::size_t yDistance = std::abs(p.y - pp.y);
                if (
                        (maxDistance_ > 0 && (xDistance > maxDistance_ ||
                            yDistance > maxDistance_)) ||
                        (minDistance_ > 0 && (xDistance < minDistance_ &&
                            yDistance < minDistance_))
                    ) {
                    if (chokePointDistantNum_ > 0 &&
                            (chokePoints_[p] || chokePoints_[pp])) {
                        if (++numWrongDistance > chokePointDistantNum_) {
                            return;
                        }
                    } else {
                        return;
                    }
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
        Point p = *it;
        if (!excludeList_[p] && !state[p]) {
            State state2(state);
            state2.addStone(p);
            initIter(it, stones - 1, state2);
        }
    } while (++it != range_.end());
}

void SubStatusForEach::cleanWorkQueue() {
    ++iters_;
    ioService_.post(std::bind(
            &SubStatusForEach::doWork, this, std::move(workQueue_)));
    workQueue_.clear(); // in case it wouldn't be moved
}

void SubStatusForEach::doWork(const std::vector<Status>& statuses)
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

void SubStatusForEach::start(std::size_t numStones,
            std::shared_ptr<const HeurCalculator> heurCalculator,
            ComplexChecker checker, boost::optional<Array<bool>> excludeList)
{
    assert(iterationState_ == IterationState::idle);
    heurCalculator_ = std::move(heurCalculator);
    checker_ = std::move(checker);
    solved_ = iters_ = 0;
    lastTicks_ = -1;
    iterationState_ = IterationState::filling;
    timeMeter_.reset();

    if (excludeList) {
        assert(excludeList->width() == table_->width());
        assert(excludeList->height() == table_->height());
        excludeList_ = std::move(*excludeList);
    } else {
        excludeList_.reset(table_->width(), table_->height(), false);
    }

    excludeList_[table_->destination()] = true;

    for (Point p: arrayRange(*table_)) {
        if (table_->wall(p)) {
            excludeList_[p] = true;
        }
    }

    initIter(range_.begin(), numStones, State());
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

void SubStatusForEach::wait(bool print)
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

            if (progressBar) {
                progressBar->draw(solved_);
            }
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

