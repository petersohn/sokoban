#ifndef MULTITHREADWORKER_HPP
#define MULTITHREADWORKER_HPP

#include "Mutexes.hpp"

#include <boost/asio/io_service.hpp>

#include <functional>
#include <vector>

class MultiThreadWorker {
public:
    using Action = std::function<void()>;
    enum class IterationState { idle, filling, working, done };
private:
    boost::asio::io_service& ioService;
    MutexType iterMutex;
    ConditionVariableType done;

    std::size_t workQueueLength;

    std::vector<Action> workQueue;
    IterationState iterationState = IterationState::idle;
    std::size_t iters = 0;
    std::size_t solved = 0;

    void cleanWorkQueue();
    void doWork(const std::vector<Action>& actions);
public:

    MultiThreadWorker(boost::asio::io_service& ioService,
            std::size_t workQueueLength):
            ioService(ioService),
            MUTEX_DECL(iterMutex),
            workQueueLength(workQueueLength) {}

    void startFilling();
    void finishFilling();
    void addAction(const Action& action);
    void wait(bool print);
    IterationState getIterationState() const { return iterationState; }
};

#endif // MULTITHREADWORKER_HPP
