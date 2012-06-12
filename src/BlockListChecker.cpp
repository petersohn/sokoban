#include "BlockListChecker.h"
#include "ComplexChecker.h"
#include "TableIterator.h"
#include <algorithm>
#include <functional>
#include <iostream>
#include <cstdlib>

BlockListChecker::BlockListChecker(Solver::Ptr solver, HeurCalculator::Ptr calculator,
		Checker::Ptr checker, int numStones, int maxDistance, int progressInterval):
		solver_(solver),
		calculator_(calculator),
		checker_(checker),
		numStones_(numStones),
		maxDistance_(maxDistance),
		dump_("blocklist.dump"),
		progressInterval_(boost::posix_time::seconds(progressInterval))
{
}

void BlockListChecker::doWork(const Status& status)
{
	std::deque<Node::Ptr> res = solver_->solve(status);
	if (res.size() == 0) {
		blockList_.add(status);
		dumpStatus(status, NULL, "Added");
	}
}

void BlockListChecker::init(FixedTable::Ptr table)
{
	std::cerr << "Calculating block list..." << std::endl;
	std::vector<Checker::Ptr> checkers{checker_, shared_from_this()};
	Checker::Ptr checker = std::make_shared<ComplexChecker>(checkers);
	TableIterator tableIterator(table, calculator_, checker,
			std::bind(&BlockListChecker::doWork, this, std::placeholders::_1),
			maxDistance_, progressInterval_);
	table_ = table;
	blockList_.clear();
	for (int n = 2; n <= numStones_; ++n) {
		std::cerr << "Stones = " << n << std::endl;
		tableIterator.iterate(n);
	}
	std::cerr << "Block list calculated. Size = " << blockList_.size() << std::endl;
	dump_.flush();
}

bool BlockListChecker::check(const Status& status, const Point& p)
{
	if (table_ != status.tablePtr()) {
		init(status.tablePtr());
	}
	return !blockList_.hasSubStatus(status, p);
}

const char* BlockListChecker::errorMessage()
{
	return "on blocklist";
}
