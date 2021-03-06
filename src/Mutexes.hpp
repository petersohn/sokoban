#ifndef MUTEXES_HPP_
#define MUTEXES_HPP_

#include <boost/thread/mutex.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/condition_variable.hpp>


#ifdef MUTEX_TRACE

#include <boost/date_time/posix_time/posix_time.hpp>
#include <string>
#include <iostream>

namespace sokoban {

template <class Mutex>
class TraceMutex {
    Mutex mutex;
    boost::posix_time::time_duration waitTime;
    unsigned uniqueNum = 0;
    unsigned sharedNum = 0;
    const char* name;
public:
    typedef Mutex MutexType;
    TraceMutex(const char* name):
        name(name)
    {
        std::cerr << "Mutex " << name << " created." << std::endl;
    }
    ~TraceMutex()
    {
        std::cerr << "Mutex " << name << " destroyed. Total waiting time: " << waitTime << " Unique locks: " << uniqueNum << " Shared locks: " << sharedNum << std::endl;
    }

    // Lockable Concept
    void lock()
    {
        if (mutex.try_lock()) {
            return;
        }
        boost::posix_time::ptime waitBegin(boost::posix_time::microsec_clock::local_time());
        mutex.lock();
        boost::posix_time::ptime waitEnd(boost::posix_time::microsec_clock::local_time());
        waitTime += waitEnd - waitBegin;
    }
    bool try_lock()
    {
        return mutex.try_lock();
    }
    void unlock()
    {
        ++uniqueNum;
        mutex.unlock();
    }

    // SharedLockable Concept
    void lock_shared()
    {
        if (mutex.try_lock_shared()) {
            return;
        }
        boost::posix_time::ptime waitBegin(boost::posix_time::microsec_clock::local_time());
        mutex.lock_shared();
        boost::posix_time::ptime waitEnd(boost::posix_time::microsec_clock::local_time());
        waitTime += waitEnd - waitBegin;
    }
    bool try_lock_shared()
    {
        return mutex.try_lock_shared();
    }
    void unlock_shared()
    {
        ++sharedNum;
        mutex.unlock_shared();
    }

};

#define MUTEX_DECL(name) name{#name}

typedef TraceMutex<boost::mutex> MutexType;
typedef TraceMutex<boost::shared_mutex> SharedMutexType;
typedef boost::condition_variable_any ConditionVariableType;

} // namespace sokoban

#else

namespace sokoban {

#define MUTEX_DECL(name) name{}

typedef boost::mutex MutexType;
typedef boost::shared_mutex SharedMutexType;
typedef boost::condition_variable ConditionVariableType;

} // namespace sokoban

#endif // #ifdef MUTEX_TRACE

#endif /* MUTEXES_HPP_ */
