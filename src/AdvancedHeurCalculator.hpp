#ifndef ADVANCEDHEURCALCULATOR_H_
#define ADVANCEDHEURCALCULATOR_H_

#include "TableHeurCalculator.hpp"
#include "Dumper/DumperFunctions.hpp"
#include "Solver.hpp"
#include <vector>
#include <iostream>
#include <fstream>
#include <boost/optional.hpp>

class AdvancedStoneCalculator {
	struct Partition
	{
		Point pos;
		Array<bool> reachable;
		float heur;
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
		std::string filename_;
		std::ofstream file_;
		void open();
	public:
		HeurDumper(std::string filename): filename_(std::move(filename)) {}
		void dumpPartition(const Table& table, const Partition& partition);
		template<class T>
		void dumpArray(const Array<T>& arr)
		{
			::dumpArray<std::string>(file_, arr);
		}
		void printText(const char* text);
	};

	Array<std::vector<Partition>> partitions_;
	Solver::Ptr solver_;
	std::size_t reverseSearchMaxDepth_;
	boost::optional<std::string> filename_;
	void init(const Table& table);

	void initPartitions(const Table& table, Point p);
public:
	AdvancedStoneCalculator(const Table& table, Solver::Ptr solver,
			std::size_t reverseSearchMaxDepth,
			const boost::optional<std::string>& filename):
		solver_(std::move(solver)),
		reverseSearchMaxDepth_(reverseSearchMaxDepth),
		filename_(filename)
	{
		assert(solver_.get());
		init(table);
	}

	AdvancedStoneCalculator(const AdvancedStoneCalculator&) = default;
	AdvancedStoneCalculator& operator=(const AdvancedStoneCalculator&) = default;
	AdvancedStoneCalculator(AdvancedStoneCalculator&&) = default;
	AdvancedStoneCalculator& operator=(AdvancedStoneCalculator&&) = default;

	float operator()(const Status& status, Point p) const;
};

using AdvancedHeurCalculator = TableHeurCalculator<AdvancedStoneCalculator>;


#endif /* ADVANCEDHEURCALCULATOR_H_ */
