#ifndef TABLEHEURCALCULATOR_H_
#define TABLEHEURCALCULATOR_H_

#include "HeurCalculator.h"
#include "Array.h"
#include "Problem.h"
#include <boost/scoped_ptr.hpp>

class ProblemHeurCalculator: public HeurCalculator {
	FixedProblem::Ptr problem_;
public:
	ProblemHeurCalculator(FixedProblem::Ptr problem);

	const Problem& problem() const { return problem_->problem(); }
};

#endif /* TABLEHEURCALCULATOR_H_ */
