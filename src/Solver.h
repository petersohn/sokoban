#ifndef SOLVER_H_
#define SOLVER_H_

#include "QueueInterfaces.h"
#include "Expander.h"
#include <deque>

class Status;
class Node;

class Solver {
public:
	typedef boost::shared_ptr<Solver> Ptr;

	std::deque<Node::Ptr> solve(Status status);
private:
	// factory methods
	virtual NodeQueue::Ptr createQueue() = 0;
	virtual Expander::Ptr createExpander() = 0;
};

#endif /* SOLVER_H_ */
