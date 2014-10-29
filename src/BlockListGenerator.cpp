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


BlockListGenerator::BlockListGenerator(Solver::Ptr solver,
		HeurCalculator::Ptr calculator, Checker::Ptr checker,
		const Options& options):
	solver_(std::move(solver)),
	calculator_(std::move(calculator)),
	checker_(std::move(checker)),
	options_(options),
	dump_("blocklist.dump"),
	threadPool_()
{
	threadPool_.setNumThreads(options.numThreads_);
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
	std::size_t decisionTreeDepth =
		options_.blocklistHeurCalculatorType_ == BlockListHeurType::decisionTree ?
			options_.blocklistDecisionTreeDepth_ : 0;
	std::cerr << "Calculating block list..." << std::endl;
	TableIterator tableIterator(table,
			std::bind(&BlockListGenerator::calculateHeurList, this, std::placeholders::_1),
			options_.blockListDistance_, options_.workQueueLength_,
			options_.reverseSearchMaxDepth_, threadPool_.getIoService());
	blockList_.clear();
	heurList_.clear();
	calculationInfos_.resize(options_.numThreads_);
	for (std::size_t n = 2; n <= options_.blockListStones_; ++n) {
		incrementalCalculator_ = n == 2 ?
			calculator_ :
				decisionTreeDepth > 0 ?
				decisionTreeHeurCalculator(decisionTreeDepth, false) :
				vectorHeurCalculator();
		for (auto& calculationInfo: calculationInfos_) {
			calculationInfo.reset(new CalculationInfo);
		}
		std::cerr << "Stones = " << n << std::endl;
		{
			util::ThreadPoolRunner runner(threadPool_);
			tableIterator.iterate(n, calculator_,
					std::make_shared<ComplexChecker>(std::vector<Checker::Ptr>{
						checker_, checker()}));
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
			boost::sort(calculationInfo->heurList_,
					[](const IncrementInfo& lhs, const IncrementInfo& rhs) {
						return lhs.difference_ > rhs.difference_;
					});
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
	}
	if (options_.maxHeurListSize_ > 0 &&
			heurList_.size() > options_.maxHeurListSize_) {
		IncrementList(heurList_.begin(),
				heurList_.begin() + options_.maxHeurListSize_).swap(heurList_);
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
			BlockListHeurListFactory{heurList_ | boost::adaptors::transformed(
					IncrementInfo::getHeurInfo) | boost::adaptors::reversed});
}


HeurCalculator::Ptr BlockListGenerator::decisionTreeHeurCalculator(std::size_t maxDepth,
		bool useChecker)
{
	assert(table_);
	return std::make_shared<DecisionTreeHeurCalculator>(
			calculator_,
			DecisionTreeHeurListFactory{
				*table_,
				heurList_ | boost::adaptors::transformed(
						IncrementInfo::getHeurInfo),
				useChecker ? checker_ : Checker::Ptr(),
				maxDepth,
				options_.numThreads_});
}

