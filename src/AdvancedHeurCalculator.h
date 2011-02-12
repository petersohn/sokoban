#ifndef ADVANCEDHEURCALCULATOR_H_
#define ADVANCEDHEURCALCULATOR_H_

#include "StoneBasedHeurCalculator.h"
#include <vector>

class AdvancedHeurCalculator: public TableHeurCalculator {
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
	HeurCalculator::Ptr calculator_;
	virtual int doCalculateStone(const Status &status, const Point &p) const;
	virtual void init();

	void initPartition(const Point &p, Array<bool> &kell,
			int &kellNum, Problem &problem);
public:
	AdvancedHeurCalculator(HeurCalculator::Ptr calculator):
		calculator_(calculator)
	{}
};

#endif /* ADVANCEDHEURCALCULATOR_H_ */
