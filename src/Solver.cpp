#include "Solver.hpp"
#include "Status/Status.hpp"
#include "Node.hpp"
#include "util/ThreadPool.hpp"
#include "Dumper/DumperFunctions.hpp"
#include "JobManager.hpp"
#include <boost/foreach.hpp>
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>

class InternalSolver {
	NodeQueue::Ptr queue_;
	Expander::Ptr expander_;
	Dumper::Ptr dumper_;
	bool parallelOuterExpand_;
	JobManager jobManager_;
	int costFgv_;
	Node::Ptr currentNode_;
public:
	InternalSolver(NodeQueue::Ptr queue, Expander::Ptr expander, Dumper::Ptr dumper,
			bool parallelOuterExpand, boost::asio::io_service& ioService):
		queue_(queue),
		expander_(expander),
		dumper_(dumper),
		parallelOuterExpand_(parallelOuterExpand),
		jobManager_(ioService),
		costFgv_(-1)
	{
		assert(queue.get() != NULL);
		assert(expander.get() != NULL);
	}

	bool expandSerial(Status& status)
	{
		expander_->expand(status, currentNode_, *queue_, dumper_);
		currentNode_ = queue_->pop();
		if (!currentNode_) {
			return false;
		}
		status.set(*currentNode_);
		return true;
	}

	void expandWithCopiedStatus(Status status, Node::Ptr currentNode)
	{
		expander_->expand(status, currentNode, *queue_, dumper_);
	}

	bool needToWait()
	{
		Node::Ptr node = queue_->peek();
		return !node || (costFgv_ >= 0 && node->costFgv() > costFgv_);
	}

	bool expandParallel(Status& status)
	{
		jobManager_.addJob(std::bind(&InternalSolver::expandWithCopiedStatus, this, status, currentNode_));
		int jobsRemaining = jobManager_.wait(std::bind(&InternalSolver::needToWait, this));
		if (jobsRemaining == 0 && !queue_->peek()) {
			return false;
		}
		currentNode_ = queue_->pop();
		if (currentNode_) {
			status.set(*currentNode_);
			costFgv_ = currentNode_->costFgv();
		}
		return true;
	}

	std::deque<Node::Ptr> solve(Status status) {
		costFgv_ = -1;
		currentNode_.reset();
		if (dumper_)
			dumper_->initialStatus(status);
		do
		{
			bool ok;
			if (parallelOuterExpand_) {
				ok = expandParallel(status);
			} else {
				ok = expandSerial(status);
			}
			if (!ok) {
				break;
			}
		} while (currentNode_->heur() > 0);
		if (parallelOuterExpand_) {
			jobManager_.waitAllJobs();
		}
		std::deque<Node::Ptr> result;
		if (currentNode_ && currentNode_->heur() == 0) {
			result = pathToRoot(currentNode_);
		}
		if (dumper_) {
			BOOST_FOREACH(Node::Ptr node, result) {
				dumper_->addToSolution(node);
			}
			dumper_->save();
		}
		return result;
	}

};

std::deque<Node::Ptr> Solver::solve(const Status &status)
{
	std::unique_ptr<util::ThreadPoolRunner> runner;
	if (numThreads_ > 0) {
		runner.reset(new util::ThreadPoolRunner(threadPool_));
	}
	InternalSolver solver(
			queueFactory_(),
			expanderFactory_(),
			dumperFactory_(),
			numThreads_ > 0,
			threadPool_.getIoService());
	return solver.solve(status);

}

