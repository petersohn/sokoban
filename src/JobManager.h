#ifndef JOBMANAGER_H_
#define JOBMANAGER_H_

#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/asio/io_service.hpp>

class JobManager {
	uint numJobs_;
	boost::asio::io_service &io_;
	MutexType jobMutex_;
	boost::condition_variable jobReady_;

	friend class DoJob;
	template<typename Func>
	class DoJob {
		JobManager* owner_;
		Func func_;
	public:
		DoJob(JobManager* owner, Func func): owner_(owner), func_(func) {}
		void operator()() {
			func_();
			{
				boost::unique_lock<MutexType> lck(owner_->jobMutex_);
				--owner_->numJobs_;
				owner_->jobReady_.notify_one();
			}
		}
	};
public:
	JobManager(boost::asio::io_service &io):
		numJobs_(0),
		io_(io)
	{
	}

	template <typename Func>
	void addJob(const Func& func)
	{
		{
			boost::unique_lock<MutexType> lck(jobMutex_);
			++numJobs_;
		}
		io_.post(DoJob<Func>(this, func));
	}

	uint numJobs()
	{
		boost::unique_lock<MutexType> lck(jobMutex_);
		return numJobs_;
	}

	template <typename Cond>
	uint wait(const Cond& cond)
	{
		boost::unique_lock<MutexType> lck(jobMutex_);
		while (numJobs_ > 0 && cond()) {
			jobReady_.wait(lck);
		}
		return numJobs_;
	}

	void waitAllJobs()
	{
		boost::unique_lock<MutexType> lck(jobMutex_);
		while (numJobs_ > 0) {
			jobReady_.wait(lck);
		}
	}
};


#endif /* JOBMANAGER_H_ */
