#include "BlockListGenerator.h"
#include "BlockListChecker.h"
#include "BlockListHeurCalculator.h"
#include "ComplexChecker.h"
#include "TableIterator.h"
#include "TableHeurCalculator.h"
#include "Checker.h"
#include <algorithm>
#include <functional>
#include <iostream>
#include <cstdlib>
#include <boost/range/algorithm.hpp>


BlockListGenerator::BlockListGenerator(Solver::Ptr solver, HeurCalculator::Ptr calculator,
		Checker::Ptr checker, int numStones, int maxDistance, int maxHeurListSize):
		solver_(solver),
		calculator_(calculator),
		checker_(checker),
		blockList_(new IndexedStatusList<int>),
		heurList_(new std::vector<HeurInfo>),
		numStones_(numStones),
		maxDistance_(maxDistance),
		maxHeurListSize_(maxHeurListSize),
		dump_("blocklist.dump")
{
}

std::deque<Node::Ptr> BlockListGenerator::doCalculateBlockList(const Status& status)
{
	std::deque<Node::Ptr> result = solver_->solve(status);
	if (result.size() == 0) {
		blockList_->add(status, 0);
		dumpStatus(status, NULL, "Blocked");
	}
	return result;
}

void BlockListGenerator::calculateBlockList(const Status& status)
{
	doCalculateBlockList(status);
}

void BlockListGenerator::calculateHeurList(const Status& status)
{
	std::deque<Node::Ptr> result = doCalculateBlockList(status);
	if (result.size() > 0) {
		int heur = calculator_->calculateStatus(status);
		int cost = result.back()->cost();
		int difference = cost - heur;
		if (difference > 0) {
			dump_ << heur << " --> " << cost << "(" << difference << ")\n";
			dumpStatus(status, NULL, "Added heur");
			heurList_->push_back(HeurInfo(VisitedStateInfo(status), cost));
		}
	}
}

void BlockListGenerator::init(const FixedTable::Ptr& table)
{
	table_ = table;
	std::cerr << "Calculating block list..." << std::endl;
	std::vector<Checker::Ptr> checkers{checker_, checker()};
	Checker::Ptr checker = std::make_shared<ComplexChecker>(checkers);
	TableIterator tableIterator(table, calculator_, checker,
			std::bind(&BlockListGenerator::calculateBlockList, this, std::placeholders::_1),
			maxDistance_);
	blockList_->clear();
	for (int n = 2; n <= numStones_; ++n) {
		if (n == numStones_) {
			tableIterator.setAction(std::bind(&BlockListGenerator::calculateHeurList, this, std::placeholders::_1));
		}
		std::cerr << "Stones = " << n << std::endl;
		tableIterator.iterate(n);
		std::cerr << "Block list size = " << blockList_->size() << std::endl;
	}
	boost::sort(*heurList_, [](const HeurInfo& left, const HeurInfo& right) { return left.heur_ > right.heur_; });
	if (maxHeurListSize_ > 0 && heurList_->size() > maxHeurListSize_) {
		heurList_->resize(maxHeurListSize_);
	}
	std::cerr << "Heur list size = " << heurList_->size() << std::endl;
	dump_.flush();
}

Checker::Ptr BlockListGenerator::checker()
{
	assert(table_);
	return std::make_shared<BlockListChecker>(blockList_, table_);
}

HeurCalculator::Ptr BlockListGenerator::heurCalculator()
{
	assert(table_);
	return std::make_shared<BlocklistHeurCalculator>(calculator_, heurList_, table_);
}

