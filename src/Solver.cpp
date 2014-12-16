#include "Solver.hpp"
#include "Status/Status.hpp"
#include "Node.hpp"
#include "util/ThreadPool.hpp"
#include "Dumper/DumperFunctions.hpp"
#include "PrioNodeQueue.hpp"
#include "Expander.hpp"
#include <boost/foreach.hpp>
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>

class InternalSolver {
	std::shared_ptr<PrioNodeQueue> queue_;
	std::shared_ptr<Expander> expander_;
	Dumper::Ptr dumper_;
	int costFgv_;
	std::shared_ptr<Node> currentNode_;
public:
	InternalSolver(std::shared_ptr<PrioNodeQueue> queue, std::shared_ptr<Expander> expander,
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

	std::deque<std::shared_ptr<Node>> solve(Status& status) {
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
		std::deque<std::shared_ptr<Node>> result;
		if (currentNode_ && currentNode_->heur() == 0) {
			result = pathToRoot(currentNode_);
		}
		if (dumper_) {
			BOOST_FOREACH(std::shared_ptr<Node> node, result) {
				dumper_->addToSolution(node);
			}
			dumper_->save();
		}
		return result;
	}

};

std::deque<std::shared_ptr<Node>> Solver::solve(Status status) const
{
	InternalSolver solver(
			queueFactory_(),
			expanderFactory_(),
			dumperFactory_());
	return solver.solve(status);

}

