#ifndef SOLVER_H_
#define SOLVER_H_

#include "Status.h"
#include "HeurCalculator.h"
#include "Node.h"
#include <deque>

class Solver {
public:
	std::deque<Node::Ptr> solve(const Status &status,
			HeurCalculator::Ptr calculator,
			bool enableLog_ = false, bool enableDump = false);

};

#endif /* SOLVER_H_ */
