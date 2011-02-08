#ifndef ADVANCEDHEURCALCULATOR_H_
#define ADVANCEDHEURCALCULATOR_H_

#include "BasicHeurCalculator.h"
#include <vector>

class AdvancedHeurCalculator: public ProblemHeurCalculator {
	struct Partition
	{
		Array<bool> reachable;
		int heur;
		Partition(size_t width, size_t height):
			reachable(width, height, false),
			heur(0)
			{}
	};


	Array<std::vector<Partition> > partitions_;
	BasicHeurCalculator::Ptr calculator_;
	virtual int doCalculate(const Status &status);
	virtual void init();

	void initPartition(const Point &p, Array<bool> &kell,
			int &kellNum, Problem &problem);
public:
	AdvancedHeurCalculator(BasicHeurCalculator::Ptr calculator):
		calculator_(calculator)
	{}
};

#endif /* ADVANCEDHEURCALCULATOR_H_ */
