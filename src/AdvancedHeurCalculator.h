#ifndef ADVANCEDHEURCALCULATOR_H_
#define ADVANCEDHEURCALCULATOR_H_

#include "TableHeurCalculator.h"
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
		void printText(const char *text);
	};

	static HeurDumper dumper;

	Array<std::vector<Partition> > partitions_;
	HeurCalculator::Ptr calculator_;
	virtual int doCalculateStone(const Status &status, const Point &p);
	virtual void init();

	void initPartition(const Point &p, Array<bool> &kell,
			int &kellNum);
public:
	AdvancedHeurCalculator(HeurCalculator::Ptr calculator):
		calculator_(calculator)
	{}
};

#endif /* ADVANCEDHEURCALCULATOR_H_ */
