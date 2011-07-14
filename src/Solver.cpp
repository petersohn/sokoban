#include "Solver.h"
#include "Status.h"
#include "Node.h"
#include <boost/foreach.hpp>

class InternalSolver {
	NodeQueue::Ptr queue_;
	Expander::Ptr expander_;
	Dumper::Ptr dumper_;
public:
	InternalSolver(NodeQueue::Ptr queue, Expander::Ptr expander, Dumper::Ptr dumper):
		queue_(queue),
		expander_(expander),
		dumper_(dumper)
	{
		assert(queue.get() != NULL);
		assert(expander.get() != NULL);
	}

	void expand(const Status &status, Node::Ptr node) {
		expander_->expand(status, node, *queue_, dumper_);
	}

	std::deque<Node::Ptr> solve(Status status) {
		Node::Ptr currentNode;
		if (dumper_)
			dumper_->initialStatus(status);
		do
		{
			expand(status, currentNode);
			currentNode = queue_->pop();
			if (currentNode.get() == NULL)
				break;
			status.set(*currentNode);
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
	InternalSolver solver(queueFactory_(), expanderFactory_(), dumperFactory_());
	return solver.solve(status);

}

