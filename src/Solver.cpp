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
	boost::asio::io_service &io_;
public:
	InternalSolver(NodeQueue::Ptr queue, Expander::Ptr expander, Dumper::Ptr dumper,
			bool parallelOuterExpand):
		queue_(queue),
		expander_(expander),
		dumper_(dumper),
		parallelOuterExpand_(parallelOuterExpand),
		jobs_(0),
		io_(ThreadPool::instance()->ioService())
	{
		assert(queue.get() != NULL);
		assert(expander.get() != NULL);
	}

	void expand(Status status, Node::Ptr node) {
		expander_->expand(status, node, *queue_, dumper_);
		if (parallelOuterExpand_) {
			boost::lock_guard<MutexType> lck(jobMutex_);
			--jobs_;
			jobReady_.notify_one();
		}
	}

	std::deque<Node::Ptr> solve(Status status) {
		Node::Ptr currentNode;
		int costFgv = -1;
		if (dumper_)
			dumper_->initialStatus(status);
		do
		{
			if (parallelOuterExpand_) {
				{
					boost::lock_guard<MutexType> lck(jobMutex_);
					++jobs_;
				}
				io_.post(boost::bind(&InternalSolver::expand, this,
						status, currentNode));
				currentNode = queue_->peek();
				{
					boost::unique_lock<MutexType> lck(jobMutex_);
					while (jobs_ > 0 && (!currentNode ||
							(costFgv >= 0 && currentNode->costFgv() > costFgv))) {
						jobReady_.wait(lck);
						currentNode = queue_->peek();
					}
					if (jobs_ == 0 && !currentNode) {
						break;
					}
				}
				currentNode = queue_->pop();
				if (currentNode) {
					status.set(*currentNode);
					costFgv = currentNode->costFgv();
				}
			} else {
				expand(status, currentNode);
				currentNode = queue_->pop();
				if (!currentNode) {
					break;
				}
				status.set(*currentNode);
			}
		} while (currentNode->heur() > 0);
		{
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

