#ifndef SOLVER_H_
#define SOLVER_H_

#include "Status.h"
#include "HeurCalculator.h"
#include "Node.h"
#include <vector>

class Solver {
	HeurCalculator::Ptr calculator_;

	std::vector<Node::Ptr> expandNodes(Node::Ptr node);
	void expandNode(Node::Ptr node, int stone, const Point &d, std::vector<Node::Ptr> &result);
public:
	Solver(HeurCalculator::Ptr calculator):
		calculator_(calculator)
	{}

	std::vector<Node::Ptr> solve(const Status &status);
};

#endif /* SOLVER_H_ */
