#ifndef TABLEHEURCALCULATOR_H_
#define TABLEHEURCALCULATOR_H_

#include "HeurCalculator.h"
#include "Array.h"
#include "Problem.h"
#include <boost/scoped_ptr.hpp>

class ProblemHeurCalculator: public HeurCalculator {
	FixedProblem::Ptr problem_;
	virtual int doCalculate(const Status &status) = 0;
	virtual void init() = 0;
public:
	ProblemHeurCalculator() {}
	virtual int calculate(const Status &status);

	const Problem& problem() const { return problem_->get(); }
	FixedProblem::Ptr problemPtr() const { return problem_; }
};

#endif /* TABLEHEURCALCULATOR_H_ */
