#include "ThreadPool.h"
#include <functional>
#include <boost/foreach.hpp>
#include <boost/exception/all.hpp>
#include <iostream>

ThreadPool::Ptr ThreadPool::instance_;

void ThreadPool::runInThread()
{
//	std::cerr << "Starting thread: " << boost::this_thread::get_id() << std::endl;
	while (true) {
		try {
			ioService_.run();
			break;
		} catch (std::exception &e) {
			std::cerr << boost::diagnostic_information(e) << std::endl;
		}
		// retry
	}
//	std::cerr << "Stopping thread: " << boost::this_thread::get_id() << std::endl;
}

void ThreadPool::numThreads(std::size_t value)
{
	if (numThreads_ == value) {
		return;
	}
	bool wasRunning = isRunning_;
	if (wasRunning) {
		wait();
	}
	numThreads_ = value;
	if (wasRunning) {
		start();
	}

}

void ThreadPool::start()
{
	if (!isRunning_ && numThreads_ > 0) {
		work_.reset(new boost::asio::io_service::work(ioService_));
		threads_.reserve(numThreads_);
		while (threads_.size() < numThreads_) {
			ThreadPtr t(new boost::thread(std::bind(&ThreadPool::runInThread, this)));
			threads_.push_back(t);
		}
		isRunning_ = true;
	}
}

void ThreadPool::wait()
{
	if (isRunning_) {
		work_.reset();
		BOOST_FOREACH(ThreadPtr t, threads_) {
			t->join();
		}
		threads_.clear();
		isRunning_ = false;
	}
}

