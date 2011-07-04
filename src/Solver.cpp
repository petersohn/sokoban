#include "Solver.h"
#include "Status.h"
#include "Node.h"

std::deque<Node::Ptr> Solver::solve(Status status)
{
	NodeQueue::Ptr queue = queueFactory_();
	assert(queue.get() != NULL);
	Expander::Ptr expander = expanderFactory_();
	assert(expander.get() != NULL);
	Dumper::Ptr dumper = dumperFactory_();
	Node::Ptr nnn = Node::create();
	Node::Ptr currentNode;
	if (dumper)
		dumper->initialStatus(status);
	do
	{
		expander->expand(status, currentNode, *queue, dumper);
		currentNode = queue->pop();
		if (currentNode.get() == NULL)
			break;
		status.set(*currentNode);
	} while (currentNode->heur() > 0);
	std::deque<Node::Ptr> result = pathToRoot(currentNode);
	if (dumper) {
		for (std::deque<Node::Ptr>::iterator it = result.begin();
				it != result.end(); ++it)
			dumper->addToSolution(*it);
		dumper->save();
	}
	return result;
}

