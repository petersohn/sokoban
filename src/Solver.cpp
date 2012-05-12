#include "Solver.h"
#include "Status.h"
#include "Node.h"
#include "ThreadPool.h"
#include "DumperFunctions.h"
#include <boost/foreach.hpp>
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>

class InternalSolver {
	NodeQueue::Ptr queue_;
	Expander::Ptr expander_;
	Dumper::Ptr dumper_;
	bool parallelOuterExpand_;
	MutexType jobMutex_;
	boost::condition_variable jobReady_;
	int jobs_;
	int costFgv_;
	Node::Ptr currentNode;
	boost::asio::io_service &io_;
public:
	InternalSolver(NodeQueue::Ptr queue, Expander::Ptr expander, Dumper::Ptr dumper,
			bool parallelOuterExpand):
		queue_(queue),
		expander_(expander),
		dumper_(dumper),
		parallelOuterExpand_(parallelOuterExpand),
		jobs_(0),
		costFgv_(-1),
		io_(ThreadPool::instance()->ioService())
	{
		assert(queue.get() != NULL);
		assert(expander.get() != NULL);
	}

	// status is deliberately copied because of thread safety
	void doExpand(Status status, Node::Ptr node) {
		expander_->expand(status, node, *queue_, dumper_);
		if (parallelOuterExpand_) {
			boost::lock_guard<MutexType> lck(jobMutex_);
			--jobs_;
			jobReady_.notify_one();
		}
	}

	bool expandSerial(Status& status)
	{
		doExpand(status, currentNode);
		currentNode = queue_->pop();
		if (!currentNode) {
			return false;
		}
		status.set(*currentNode);
		return true;
	}

	bool expandParallel(Status& status)
	{
		{
			boost::lock_guard<MutexType> lck(jobMutex_);
			++jobs_;
		}
		io_.post(boost::bind(&InternalSolver::doExpand, this,
				status, currentNode));
		currentNode = queue_->peek();
		{
			boost::unique_lock<MutexType> lck(jobMutex_);
			while (jobs_ > 0 && (!currentNode ||
					(costFgv_ >= 0 && currentNode->costFgv() > costFgv_))) {
				jobReady_.wait(lck);
				currentNode = queue_->peek();
			}
			if (jobs_ == 0 && !currentNode) {
				return false;
			}
		}
		currentNode = queue_->pop();
		if (currentNode) {
			status.set(*currentNode);
			costFgv_ = currentNode->costFgv();
		}
		return true;
	}

	std::deque<Node::Ptr> solve(Status status) {
		costFgv_ = -1;
		currentNode.reset();
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
		} while (currentNode->heur() > 0);
		if (parallelOuterExpand_) {
			boost::unique_lock<MutexType> lck(jobMutex_);
			while (jobs_ > 0) {
				jobReady_.wait(lck);
			}
		}
		std::deque<Node::Ptr> result = pathToRoot(currentNode);
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
	InternalSolver solver(queueFactory_(), expanderFactory_(), dumperFactory_(), multithread_);
	return solver.solve(status);

}

