#ifndef BASICSOLVER_H_
#define BASICSOLVER_H_

#include "Solver.h"

class BasicSolver: public Solver {
	virtual NodeQueue::Ptr createQueue();
	virtual Expander::Ptr createExpander();
};

#endif /* BASICSOLVER_H_ */
