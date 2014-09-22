#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <memory>
#include <boost/noncopyable.hpp>
#include <vector>

class ThreadPool: public boost::noncopyable {
public:
	typedef std::shared_ptr<ThreadPool> Ptr;
private:
	typedef std::shared_ptr<boost::thread> ThreadPtr;

	boost::asio::io_service ioService_;
	std::auto_ptr<boost::asio::io_service::work> work_;
	std::vector<ThreadPtr> threads_;
	bool isRunning_;
	std::size_t numThreads_;

	void runInThread();
public:
	boost::asio::io_service& ioService() { return ioService_; }
	std::size_t numThreads() const { return numThreads_; }
	void numThreads(std::size_t value);
	bool isRunning() const { return isRunning_; }
	void start();
	void wait();

	ThreadPool():
		isRunning_(false),
		numThreads_(0)
	{
	}
};

class ThreadPoolRunner {
	ThreadPool& threadPool_;
public:
	ThreadPoolRunner(ThreadPool& threadPool):
		threadPool_(threadPool)
	{
		threadPool_.start();
	}
	~ThreadPoolRunner()
	{
		threadPool_.wait();
	}
};

#endif /* THREADPOOL_H_ */
