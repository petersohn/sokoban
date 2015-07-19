#ifndef MULTITHREADWORKER_HPP
#define MULTITHREADWORKER_HPP

#include "Mutexes.hpp"

#include <functional>
#include <vector>

namespace boost { namespace asio { class io_service; }}

class MultiThreadWorker {
public:
    using Action = std::function<void()>;
    enum class IterationState { idle, filling, working, done };
private:
    boost::asio::io_service& ioService;
    MutexType iterMutex;
    ConditionVariableType doneCondition;
    ConditionVariableType synchronizationCondition;

    std::size_t workQueueLength;

    std::vector<Action> workQueue;
    IterationState iterationState = IterationState::idle;
    std::size_t iters = 0;
    std::size_t solved = 0;
    std::size_t numRunning = 0;
    bool synchronizationRequested = false;

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
    void synchronize(const std::function<void()>& function);
};

#endif // MULTITHREADWORKER_HPP
