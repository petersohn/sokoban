#include "BlockListChecker.h"
#include "ComplexChecker.h"
#include "TableIterator.h"
#include <algorithm>
#include <functional>
#include <iostream>
#include <cstdlib>

BlockListChecker::BlockListChecker(Solver::Ptr solver, HeurCalculator::Ptr calculator,
		Checker::Ptr checker, int numStones, int maxDistance):
		solver_(solver),
		calculator_(calculator),
		checker_(checker),
		numStones_(numStones),
		maxDistance_(maxDistance),
		dump_("blocklist.dump")
{
}

std::deque<Node::Ptr> BlockListChecker::doCalculateBlockList(const Status& status)
{
	std::deque<Node::Ptr> result = solver_->solve(status);
	if (result.size() == 0) {
		blockList_.add(status, 0);
		dumpStatus(status, NULL, "Blocked");
	}
	return result;
}

void BlockListChecker::calculateBlockList(const Status& status)
{
	doCalculateBlockList(status);
}

void BlockListChecker::calculateHeurList(const Status& status)
{
	std::deque<Node::Ptr> res = solver_->solve(status);
	if (res.size() == 0) {
		blockList_.add(status, 0);
		dumpStatus(status, NULL, "Blocked");
	}
}

void BlockListChecker::init()
{
	std::cerr << "Calculating block list..." << std::endl;
	std::vector<Checker::Ptr> checkers{checker_, shared_from_this()};
	Checker::Ptr checker = std::make_shared<ComplexChecker>(checkers);
	TableIterator tableIterator(table, calculator_, checker,
			std::bind(&BlockListChecker::calculateBlockList, this, std::placeholders::_1),
			maxDistance_);
	blockList_.clear();
	for (int n = 2; n <= numStones_; ++n) {
		std::cerr << "Stones = " << n << std::endl;
		tableIterator.iterate(n);
		std::cerr << "Block list size = " << blockList_.size() << std::endl;
	}
	dump_.flush();
}

bool BlockListChecker::check(const Status& status, const Point& p)
{
	if (tablePtr() != status.tablePtr()) {
		setTable(status.tablePtr());
	}
	return !blockList_.hasSubStatus(status, p);
}

const char* BlockListChecker::errorMessage()
{
	return "on blocklist";
}
