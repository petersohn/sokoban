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
	bool multithread_;
	MutexType jobMutex_;
	boost::condition_variable jobReady_;
	int jobs_;
	boost::asio::io_service &io_;
public:
	InternalSolver(NodeQueue::Ptr queue, Expander::Ptr expander, Dumper::Ptr dumper,
			bool multithread):
		queue_(queue),
		expander_(expander),
		dumper_(dumper),
		multithread_(multithread),
		jobs_(0),
		io_(ThreadPool::instance()->ioService())
	{
		assert(queue.get() != NULL);
		assert(expander.get() != NULL);
	}

	void expand(const Status &status, Node::Ptr node) {
		expander_->expand(status, node, *queue_, dumper_);
		if (multithread_) {
			boost::lock_guard<MutexType> lck(jobMutex_);
			--jobs_;
			std::cerr << "==== -JOB- ====" << std::endl;
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
			if (multithread_) {
				{
					boost::lock_guard<MutexType> lck(jobMutex_);
					++jobs_;
					std::cerr << "==== +JOB+ ====" << std::endl;
				}
				if (currentNode)
					dumpNode(std::cerr, status.tablePtr(), *currentNode, "Expanding");
				io_.post(boost::bind(&InternalSolver::expand, this,
						boost::ref(status), currentNode));
				currentNode = queue_->peek();
				if (currentNode)
					dumpNode(std::cerr, status.tablePtr(), *currentNode, "Considering");
				{
					boost::unique_lock<MutexType> lck(jobMutex_);
					while (jobs_ > 0 && (!currentNode ||
							costFgv >= 0 && currentNode->costFgv() > costFgv)) {
						std::cerr << "Waiting. J: " << jobs_ << "  C: " << costFgv << "  NC: " <<
								(currentNode ? boost::lexical_cast<std::string>(currentNode->costFgv()) : "-") << std::endl;
						jobReady_.wait(lck);
						currentNode = queue_->peek();
					}
					std::cerr << "J: " << jobs_ << "  N: " << queue_->size() << std::endl;
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

