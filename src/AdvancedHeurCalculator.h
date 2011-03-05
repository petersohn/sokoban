#ifndef ADVANCEDHEURCALCULATOR_H_
#define ADVANCEDHEURCALCULATOR_H_

#include "TableHeurCalculator.h"
#include "Dumper.h"
#include "Solver.h"
#include <vector>
#include <iostream>
#include <fstream>

class AdvancedHeurCalculator: public TableHeurCalculator {
	struct Partition
	{
		Point pos;
		Array<bool> reachable;
		int heur;
		Partition(size_t width, size_t height):
			reachable(width, height, false),
			heur(0)
			{}
	};
	class HeurDumper {
		std::ofstream file_;
		AdvancedHeurCalculator *owner_;
		void open();
	public:
		void dumpHeur(Array<int> values);
		void dumpPartition(const AdvancedHeurCalculator &calc, const Partition &part);
		template<class T>
		void dumpArray(const Array<T> arr);
		void printText(const char *text);
	};

	static HeurDumper dumper;

	Array<std::vector<Partition> > partitions_;
	Solver::Ptr solver_;
	virtual int doCalculateStone(const Status &status, const Point &p);
	virtual void init();

	void initPartition(const Point &p, Array<bool> &kell,
			int &kellNum);
public:
	AdvancedHeurCalculator(Solver::Ptr solver):
		solver_(solver)
	{
		assert(solver_.get() != NULL);
	}
};


template<class T>
void AdvancedHeurCalculator::HeurDumper::dumpArray(const Array<T> arr) {
	::dumpArray<std::string>(file_, arr);
}

#endif /* ADVANCEDHEURCALCULATOR_H_ */
