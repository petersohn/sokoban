#include "Solver.h"
#include "Status.h"
#include "Node.h"

std::deque<Node::Ptr> Solver::solve(Status status)
{
	NodeQueue::Ptr queue = createQueue();
	assert(queue.get() != NULL);
	Expander::Ptr expander = createExpander();
	assert(expander.get() != NULL);
	Node::Ptr nnn = Node::create();
	Node::Ptr currentNode;
	addVisitedState(status, calculator_->calculateStatus(status));
	do
	{
		expander->expand(status, currentNode, *queue);
		currentNode = queue.pop();
		if (currentNode.get() == NULL)
			break;
		status.set(*currentNode);
	} while (currentNode->heur() > 0);
	std::deque<Node::Ptr> result = pathToRoot(currentNode);
	return result;
}

