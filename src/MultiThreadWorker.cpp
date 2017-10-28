#include "MultiThreadWorker.hpp"
#include "ProgressBar.hpp"

#include "util/Finally.hpp"

#include <boost/asio/io_service.hpp>
#include <boost/thread.hpp>

#include <memory>

namespace sokoban {

void MultiThreadWorker::startFilling()
{
    assert(iterationState == IterationState::idle);
    solved = 0;
    iters = 0;
    iterationState = IterationState::filling;
}

void MultiThreadWorker::finishFilling()
{
    cleanWorkQueue();

    boost::unique_lock<MutexType> lock(iterMutex);
    assert(iterationState == IterationState::filling);

    if (solved == iters) {
        iterationState = IterationState::done;
    } else {
        iterationState = IterationState::working;
    }
}

void MultiThreadWorker::addAction(const Action& action)
{
    assert(iterationState == IterationState::filling);
    workQueue.push_back(action);

    if (workQueue.size() == workQueueLength) {
        cleanWorkQueue();
    }
}

void MultiThreadWorker::cleanWorkQueue() {
    ++iters;
    ioService.post(std::bind(
            &MultiThreadWorker::doWork, this, std::move(workQueue)));
    workQueue.clear(); // in case it wouldn't be moved
}

void MultiThreadWorker::doWork(const std::vector<Action>& actions) {
    {
        boost::unique_lock<MutexType> lock(iterMutex);
        ++numRunning;
    }

    for (const auto& action: actions) {
        action();
    }

    boost::unique_lock<MutexType> lock(iterMutex);
    ++solved;
    --numRunning;

    if (iterationState == IterationState::working && solved == iters) {
        iterationState = IterationState::done;

    }

    doneCondition.notify_all();

    while (synchronizationRequested) {
        synchronizationCondition.wait(lock);
    }
}

void MultiThreadWorker::wait(bool print)
{
    if (iterationState == IterationState::idle) {
        return;
    }

    {
        std::unique_ptr<ProgressBar> progressBar;

        boost::unique_lock<MutexType> lock(iterMutex);
        while (iterationState != IterationState::done) {
            doneCondition.wait(lock);
            auto currentIterationState = iterationState;
            auto currentSolved = solved;
            lock.unlock();

            if (print && !progressBar &&
                    currentIterationState == IterationState::working) {
                progressBar = std::make_unique<ProgressBar>(iters);
            }

            if (progressBar) {
                progressBar->draw(currentSolved);
            }

            lock.lock();
        }
    }

    iterationState = IterationState::idle;
}

void MultiThreadWorker::synchronize(const std::function<void()>& function)
{
    boost::unique_lock<MutexType> lock(iterMutex);
    synchronizationRequested = true;
    auto guard = util::finally(
            [this]() {
                synchronizationRequested = false;
                synchronizationCondition.notify_all();
            });
    while (numRunning != 0) {
        doneCondition.wait(lock);
    }
    function();
}

} // namespace sokoban
