#include "Solver.h"
#include "Status.h"
#include "Node.h"

std::deque<Node::Ptr> Solver::solve(Status status)
{
	NodeQueue::Ptr queue = queueFactory_();
	assert(queue.get() != NULL);
	Expander::Ptr expander = expanderFactory_();
	assert(expander.get() != NULL);
	Node::Ptr nnn = Node::create();
	Node::Ptr currentNode;
	do
	{
		if (dumper_ && currentNode)
			dumper_->expand(currentNode);
		expander->expand(status, currentNode, *queue);
		currentNode = queue->pop();
		if (currentNode.get() == NULL)
			break;
		status.set(*currentNode);
	} while (currentNode->heur() > 0);
	std::deque<Node::Ptr> result = pathToRoot(currentNode);
	return result;
}

