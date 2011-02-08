#ifndef SOLVER_H_
#define SOLVER_H_

#include "Status.h"
#include "HeurCalculator.h"
#include "Node.h"
#include <vector>

class Solver {
	HeurCalculator::Ptr calculator_;

public:
	Solver(HeurCalculator::Ptr calculator):
		problem_(problem),
		calculator_(calculator)
	{}

	std::vector<Node::Ptr> solve(const Status &status);
};

#endif /* SOLVER_H_ */
