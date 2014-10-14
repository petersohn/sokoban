#include "BlockListGenerator.hpp"
#include "BlockListChecker.hpp"
#include "BlockListHeurCalculator.hpp"
#include "DecisionTreeHeurCalculator.hpp"
#include "ComplexChecker.hpp"
#include "TableIterator.hpp"
#include "TableHeurCalculator.hpp"
#include "Checker.hpp"
#include <algorithm>
#include <functional>
#include <iostream>
#include <cstdlib>
#include <boost/range/algorithm.hpp>


BlockListGenerator::BlockListGenerator(Solver::Ptr solver, HeurCalculator::Ptr calculator,
		Checker::Ptr checker, std::size_t numStones, std::size_t maxDistance,
		std::size_t maxHeurListSize, std::size_t numThreads):
	solver_(std::move(solver)),
	calculator_(std::move(calculator)),
	checker_(std::move(checker)),
	numStones_(numStones),
	maxDistance_(maxDistance),
	maxHeurListSize_(maxHeurListSize),
	dump_("blocklist.dump"),
	threadPool_(),
	numThreads_(numThreads)
{
	threadPool_.setNumThreads(numThreads);
}

std::deque<Node::Ptr> BlockListGenerator::doCalculateBlockList(const Status& status)
{
	std::deque<Node::Ptr> result = solver_->solve(status);
	if (result.empty()) {
		auto threadId = *util::ThreadPool::getCurrentThreadId();
		calculationInfos_[threadId]->blockList_.push_back(status);
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
			auto threadId = *util::ThreadPool::getCurrentThreadId();
			calculationInfos_[threadId]->dump_ <<
					heur << " --> " << cost << "(" << difference << ")\n";
			dumpStatus(status, NULL, "Added heur");
			HeurInfo heurInfo{status, cost};
			calculationInfos_[threadId]->heurList_.push_back(
					IncrementInfo{std::move(heurInfo), difference});
		}
	}
}

void BlockListGenerator::init(const Table& table)
{
	table_ = &table;
	std::cerr << "Calculating block list..." << std::endl;
	std::vector<Checker::Ptr> checkers{checker_, checker()};
	Checker::Ptr checker = std::make_shared<ComplexChecker>(checkers);
	TableIterator tableIterator(table, calculator_, checker,
			std::bind(&BlockListGenerator::calculateHeurList, this, std::placeholders::_1),
			maxDistance_, threadPool_);
	blockList_.clear();
	heurList_.clear();
	incrementalCalculator_ = calculator_;
	calculationInfos_.resize(numThreads_);
	for (std::size_t n = 2; n <= numStones_; ++n) {
		for (auto& calculationInfo: calculationInfos_) {
			calculationInfo.reset(new CalculationInfo);
		}
		std::cerr << "Stones = " << n << std::endl;
		{
			util::ThreadPoolRunner runner(threadPool_);
			tableIterator.iterate(n);
		}
		for (const auto& calculationInfo: calculationInfos_) {
			dump_ << calculationInfo->dump_.str();

			for (const auto& status: calculationInfo->blockList_) {
				blockList_.add(status, 0);
			}

			heurList_.reserve(heurList_.size() +
					calculationInfo->heurList_.size());
			std::move(calculationInfo->heurList_.begin(),
					calculationInfo->heurList_.end(),
					std::back_inserter(heurList_));
		}
		std::cerr << "Block list size = " << blockList_.size() << std::endl;
		std::cerr << "Heur list size = " << heurList_.size() << std::endl;
		boost::sort(heurList_, [](const IncrementInfo& left, const IncrementInfo& right)
			{
				return left.difference_ > right.difference_ ||
						(left.difference_ == right.difference_ &&
						left.heurInfo_.first.state().size() <
						right.heurInfo_.first.state().size()
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
	return std::make_shared<BlockListChecker>(blockList_);
}

HeurCalculator::Ptr BlockListGenerator::vectorHeurCalculator()
{
	assert(table_);
	return std::make_shared<BlocklistHeurCalculator>(
			calculator_,
			BlockListHeurList{heurList_ | boost::adaptors::transformed(
					IncrementInfo::getHeurInfo)});
}


HeurCalculator::Ptr BlockListGenerator::decisionTreeHeurCalculator(std::size_t maxDepth,
		bool useChecker)
{
	assert(table_);
	return std::make_shared<DecisionTreeHeurCalculator>(
			calculator_,
			DecisionTreeHeurList{
				*table_,
				heurList_ | boost::adaptors::transformed(
						IncrementInfo::getHeurInfo),
				useChecker ? checker_ : Checker::Ptr(),
				maxDepth,
				numThreads_});
}

