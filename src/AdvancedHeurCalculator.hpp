#ifndef ADVANCEDHEURCALCULATOR_H_
#define ADVANCEDHEURCALCULATOR_H_

#include "TableHeurCalculator.hpp"
#include "Dumper/DumperFunctions.hpp"
#include "Solver.hpp"
#include <vector>
#include <iostream>
#include <fstream>

class AdvancedStoneCalculator {
	struct Partition
	{
		Point pos;
		Array<bool> reachable;
		int heur;
		Partition(size_t width, size_t height):
			reachable(width, height, false),
			heur(0)
			{}
		Partition(const Partition&) = default;
		Partition& operator=(const Partition&) = default;
		Partition(Partition&&) = default;
		Partition& operator=(Partition&&) = default;
	};
	class HeurDumper {
		std::ofstream file_;
		void open();
	public:
		void dumpPartition(const FixedTable::Ptr& table, const Partition& partition);
		template<class T>
		void dumpArray(const Array<T>& arr)
		{
			::dumpArray<std::string>(file_, arr);
		}
		void printText(const char* text);
	};

	static HeurDumper dumper;

	Array<std::vector<Partition>> partitions_;
	Solver::Ptr solver_;
	bool useDumper_;
	void init(const FixedTable::Ptr& table);

	void initPartitions(const FixedTable::Ptr& table, Point p);
public:
	AdvancedStoneCalculator(const FixedTable::Ptr& table, Solver::Ptr solver, bool useDumper = true):
		solver_(std::move(solver)),
		useDumper_(useDumper)
	{
		assert(solver_.get());
		init(table);
	}

	AdvancedStoneCalculator(const AdvancedStoneCalculator&) = default;
	AdvancedStoneCalculator& operator=(const AdvancedStoneCalculator&) = default;
	AdvancedStoneCalculator(AdvancedStoneCalculator&&) = default;
	AdvancedStoneCalculator& operator=(AdvancedStoneCalculator&&) = default;

	int operator()(const Status& status, Point p);
};

using AdvancedHeurCalculator = TableHeurCalculator<AdvancedStoneCalculator>;


#endif /* ADVANCEDHEURCALCULATOR_H_ */
