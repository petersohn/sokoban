#include "BlockListGenerator.h"
#include "BlockListChecker.h"
#include "ComplexChecker.h"
#include "TableIterator.h"
#include "TableHeurCalculator.h"
#include "Checker.h"
#include <algorithm>
#include <functional>
#include <iostream>
#include <cstdlib>


BlockListGenerator::BlockListGenerator(Solver::Ptr solver, HeurCalculator::Ptr calculator,
		Checker::Ptr checker, int numStones, int maxDistance):
		solver_(solver),
		calculator_(calculator),
		checker_(checker),
		blockList_(new IndexedStatusList<int>),
		numStones_(numStones),
		maxDistance_(maxDistance),
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

//void BlockListGenerator::calculateHeurList(const Status& status)
//{
//	std::deque<Node::Ptr> res = solver_->solve(status);
//	if (res.size() == 0) {
//		blockList_.add(status, 0);
//		dumpStatus(status, NULL, "Blocked");
//	}
//}

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
		std::cerr << "Stones = " << n << std::endl;
		tableIterator.iterate(n);
		std::cerr << "Block list size = " << blockList_->size() << std::endl;
	}
	dump_.flush();
}

Checker::Ptr BlockListGenerator::checker()
{
	assert(table_);
	return std::make_shared<BlockListChecker>(blockList_, table_);
}
