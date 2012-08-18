#include "BlockListGenerator.h"
#include "BlockListChecker.h"
#include "BlockListHeurCalculator.h"
#include "DecisionTreeHeurCalculator.h"
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
		Checker::Ptr checker, int numStones, int maxDistance, int maxHeurListSize,
		int numThreads):
		solver_(solver),
		calculator_(calculator),
		checker_(checker),
		blockList_(new IndexedStatusList<int>),
		numStones_(numStones),
		maxDistance_(maxDistance),
		maxHeurListSize_(maxHeurListSize),
		dump_("blocklist.dump"),
		threadPool_(),
		numThreads_(numThreads)
{
	threadPool_.numThreads(numThreads);
}

std::deque<Node::Ptr> BlockListGenerator::doCalculateBlockList(const Status& status)
{
	std::deque<Node::Ptr> result = solver_->solve(status);
	if (result.empty()) {
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
	if (!result.empty()) {
		int heur = incrementalCalculator_->calculateStatus(status);
		int cost = result.back()->cost();
		int difference = cost - heur;
		if (difference > 0) {
			dump_ << heur << " --> " << cost << "(" << difference << ")\n";
			dumpStatus(status, NULL, "Added heur");
			HeurInfoConstPtr heurInfo = std::make_shared<HeurInfo>(status, cost);
			heurList_.push_back(IncrementInfo(heurInfo, difference));
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
			std::bind(&BlockListGenerator::calculateHeurList, this, std::placeholders::_1),
			maxDistance_, threadPool_);
	blockList_->clear();
	heurList_.clear();
	incrementalCalculator_ = calculator_;
	for (int n = 2; n <= numStones_; ++n) {
		std::cerr << "Stones = " << n << std::endl;
		{
			ThreadPoolRunner runner(threadPool_);
			tableIterator.iterate(n);
		}
		std::cerr << "Block list size = " << blockList_->size() << std::endl;
		std::cerr << "Heur list size = " << heurList_.size() << std::endl;
		boost::sort(heurList_, [](const IncrementInfo& left, const IncrementInfo& right)
			{
				return left.difference_ > right.difference_ ||
						(left.difference_ == right.difference_ &&
						left.heurInfo_->first.state().size() <
						right.heurInfo_->first.state().size()
						);
			});
		incrementalCalculator_ = vectorHeurCalculator();
	}
	if (maxHeurListSize_ > 0 && heurList_.size() > maxHeurListSize_) {
		IncrementList(heurList_.begin(), heurList_.begin() + maxHeurListSize_).swap(heurList_);
	}
	std::cerr << "Heur list size = " << heurList_.size() << std::endl;
	dump_.flush();
}

Checker::Ptr BlockListGenerator::checker()
{
	assert(table_);
	return std::make_shared<BlockListChecker>(blockList_, table_);
}

HeurCalculator::Ptr BlockListGenerator::vectorHeurCalculator()
{
	assert(table_);
	return std::make_shared<BlocklistHeurCalculator>(
			calculator_,
			heurList_ | boost::adaptors::transformed(IncrementInfo::getHeurInfo),
			table_);
}


HeurCalculator::Ptr BlockListGenerator::decisionTreeHeurCalculator(int maxDepth, bool useChecker)
{
	assert(table_);
	return std::make_shared<DecisionTreeHeurCalculator>(
			calculator_,
			heurList_ | boost::adaptors::transformed(IncrementInfo::getHeurInfo),
			useChecker ? checker_ : Checker::Ptr(),
			table_,
			maxDepth,
			numThreads_);
}

