#ifndef ADVANCEDHEURCALCULATOR_H_
#define ADVANCEDHEURCALCULATOR_H_

#include "TableHeurCalculator.hpp"
#include "Dumper/DumperFunctions.hpp"
#include "Solver.hpp"
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
	bool useDumper_;
	virtual int doCalculateStone(const Status &status, Point p);
	virtual void init();

	void initPartitions(Point p);
public:
	AdvancedHeurCalculator(Solver::Ptr solver, bool useDumper = true):
		solver_(std::move(solver)),
		useDumper_(useDumper)
	{
		assert(solver_.get());
	}
};


template<class T>
void AdvancedHeurCalculator::HeurDumper::dumpArray(const Array<T> arr) {
	::dumpArray<std::string>(file_, arr);
}

#endif /* ADVANCEDHEURCALCULATOR_H_ */
