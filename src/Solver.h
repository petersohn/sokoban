#ifndef SOLVER_H_
#define SOLVER_H_

#include "Problem.h"
#include "HeurCalculator.h"
#include "Node.h"
#include <vector>

class Solver {
	FixedProblem::Ptr problem_;
	HeurCalculator::Ptr calculator_;

public:
	Solver(FixedProblem::Ptr problem, HeurCalculator::Ptr calculator):
		problem_(problem),
		calculator_(calculator)
	{}

	std::vector<Node::Ptr> solve();
};

#endif /* SOLVER_H_ */
