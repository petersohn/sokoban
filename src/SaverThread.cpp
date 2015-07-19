#include "SaverThread.hpp"

void SaverThread::start()
{
    stop();

    saveTime_ = boost::asio::time_traits<boost::posix_time::ptime>::now();
    startSaveTimer();
    running = true;
    thread = std::make_unique<boost::thread>(
            [this]() {
                ioService.run();
            });
}

void SaverThread::stop()
{
    if (thread) {
        running = false;
        saveTimer_.cancel();
        thread->join();
        thread.reset();
        ioService.reset();
    }
}

void SaverThread::startSaveTimer()
{
    saveTime_ += saveInterval;
    saveTimer_.expires_at(saveTime_);
    saveTimer_.async_wait(
            [this](boost::system::error_code errorCode) {
                if (!errorCode && running) {
                    saver();
                    startSaveTimer();
                }
            });
}
