#include "Solver.hpp"
#include "Status/Status.hpp"
#include "Node.hpp"
#include "util/ThreadPool.hpp"
#include "Dumper/DumperFunctions.hpp"
#include <boost/foreach.hpp>
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>

class InternalSolver {
	NodeQueue::Ptr queue_;
	Expander::Ptr expander_;
	Dumper::Ptr dumper_;
	int costFgv_;
	Node::Ptr currentNode_;
public:
	InternalSolver(NodeQueue::Ptr queue, Expander::Ptr expander,
			Dumper::Ptr dumper):
		queue_(std::move(queue)),
		expander_(std::move(expander)),
		dumper_(dumper),
		costFgv_(-1)
	{
		assert(queue_.get() != NULL);
		assert(expander_.get() != NULL);
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

	std::deque<Node::Ptr> solve(Status& status) {
		costFgv_ = -1;
		currentNode_.reset();
		if (dumper_)
			dumper_->initialStatus(status);
		do
		{
			if (!expandSerial(status)) {
				break;
			}
		} while (currentNode_->heur() > 0);
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

std::deque<Node::Ptr> Solver::solve(Status status) const
{
	InternalSolver solver(
			queueFactory_(),
			expanderFactory_(),
			dumperFactory_());
	return solver.solve(status);

}

