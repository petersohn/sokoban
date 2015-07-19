#ifndef SAVERTHREAD_HPP
#define SAVERTHREAD_HPP

#include "Mutexes.hpp"

#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>

#include <atomic>
#include <functional>
#include <memory>

class SaverThread {
public:
    SaverThread(std::function<void()> saver,
            boost::posix_time::time_duration saveInterval):
        saver(saver), saveInterval(saveInterval)
    {}

    void start();
    void stop();

private:
    std::function<void()> saver;
    boost::posix_time::time_duration saveInterval;

    boost::asio::io_service ioService;
    boost::asio::deadline_timer saveTimer_{ioService};
    boost::posix_time::ptime saveTime_;
    std::unique_ptr<boost::thread> thread;
    std::atomic<bool> running{false};

    void startSaveTimer();
};


#endif // SAVERTHREAD_HPP
