#include "HeurCalculator/BlockListGenerator.hpp"
#include "Checker/BlockListChecker.hpp"
#include "HeurCalculator/BlockListHeurCalculator.hpp"
#include "HeurCalculator/DecisionTreeHeurCalculator.hpp"
#include "SubStatusForEach.hpp"
#include "HeurCalculator/TableHeurCalculator.hpp"
#include "HeurCalculator/HeurCalculator.hpp"
#include "Dumper/DumperFunctions.hpp"
#include "Solver.hpp"
#include "HeurCalculator/HeurInfo.hpp"
#include "ChokePointFinder.hpp"
#include <algorithm>
#include <functional>
#include <iostream>
#include <cstdlib>
#include <boost/range/algorithm.hpp>

struct IncrementInfo {
    HeurInfo heurInfo_;
    float difference_;
    IncrementInfo(HeurInfo heurInfo, float difference):
        heurInfo_(std::move(heurInfo)),
        difference_(difference)
    {}
    IncrementInfo(const IncrementInfo& ) = default;
    IncrementInfo(IncrementInfo&& ) = default;
    IncrementInfo& operator=(const IncrementInfo& ) = default;
    IncrementInfo& operator=(IncrementInfo&& ) = default;
    static const HeurInfo& getHeurInfo(const IncrementInfo& incrementInfo) {
        return incrementInfo.heurInfo_;
    }
};


BlockListGenerator::BlockListGenerator(std::unique_ptr<const Solver> solver,
        std::shared_ptr<const HeurCalculator> calculator, ComplexChecker checker,
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

BlockListGenerator::~BlockListGenerator()
{}

std::deque<std::shared_ptr<Node>> BlockListGenerator::doCalculateBlockList(const Status& status)
{
    std::deque<std::shared_ptr<Node>> result = solver_->solve(status);
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
    std::deque<std::shared_ptr<Node>> result = doCalculateBlockList(status);
    if (!result.empty()) {
        float heur = incrementalCalculator_->calculateStatus(status);
        float cost = result.back()->cost();
        float difference = cost - heur;
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
    Array<bool> chokePoints;

    util::TimeMeter timeMeter;
    if (options_.chokePointNum_ > 0) {
        chokePoints = findChokePoints(table, options_, calculator_,
                ComplexChecker{checker_}, true);
        chokePointFinderTime_ = timeMeter.data();
        Status chokePointStatus{table};

        for (Point p: arrayRange(chokePoints)) {
            if (chokePoints[p]) {
                chokePointStatus.addStone(p);
            }
        }

        ::dumpStatus(std::cerr, chokePointStatus, "Choke points");
    }

    std::cerr << "Calculating block list..." << std::endl;
    SubStatusForEach subStatusForEach(table,
            std::bind(&BlockListGenerator::calculateHeurList, this, std::placeholders::_1),
            SubStatusForEach::MinDistance{0},
            SubStatusForEach::MaxDistance{options_.blockListDistance_},
            SubStatusForEach::ChokePointDistantNum{options_.chokePointNum_ > 0 ?
                    options_.chokePointDistantNum_ : 0}, chokePoints,
            SubStatusForEach::WorkQueueLength{options_.workQueueLength_},
            SubStatusForEach::ReverseSearchMaxDepth{options_.reverseSearchMaxDepth_},
            threadPool_.getIoService());

    blockList_.clear();
    heurList_.clear();
    calculationInfos_.resize(options_.numThreads_);
    timeMeter.reset();

    for (std::size_t n = 2; n <= options_.blockListStones_; ++n) {
        incrementalCalculator_ = n == 2 ?
            calculator_ :
                decisionTreeDepth > 0 ?
                decisionTreeHeurCalculator(decisionTreeDepth, false, 1.0f) :
                vectorHeurCalculator(1.0f);

        for (auto& calculationInfo: calculationInfos_) {
            calculationInfo = std::make_unique<CalculationInfo>();
        }

        std::cerr << "Stones = " << n << std::endl;

        {
            util::ThreadPoolRunner runner(threadPool_);
            ComplexChecker actualChecker{checker_};
            actualChecker.append(checker());
            subStatusForEach.start(n, calculator_, actualChecker);
            subStatusForEach.wait(true);
        }

        for (const auto& calculationInfo: calculationInfos_) {
            dump_ << calculationInfo->dump_.str();

            for (const auto& status: calculationInfo->blockList_) {
                blockList_.add(status);
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

    iteratingTime_ = timeMeter.data();
    if (options_.maxHeurListSize_ > 0 &&
            heurList_.size() > options_.maxHeurListSize_) {
        IncrementList(heurList_.begin(),
                heurList_.begin() + options_.maxHeurListSize_).swap(heurList_);
    }

    std::cerr << "Heur list size = " << heurList_.size() << std::endl;
    dump_.flush();
}

std::shared_ptr<const Checker> BlockListGenerator::checker()
{
    assert(table_);
    return std::make_shared<BlockListChecker>(blockList_);
}

std::shared_ptr<const HeurCalculator> BlockListGenerator::vectorHeurCalculator(float heurMultiplier)
{
    assert(table_);
    return std::make_shared<BlocklistHeurCalculator>(
            heurMultiplier, calculator_,
            BlockListHeurListFactory{heurList_ | boost::adaptors::transformed(
                    IncrementInfo::getHeurInfo) | boost::adaptors::reversed});
}


std::shared_ptr<const HeurCalculator> BlockListGenerator::decisionTreeHeurCalculator(
        std::size_t maxDepth, bool useChecker, float heurMultiplier)
{
    assert(table_);
    boost::optional<ComplexChecker> checker;
    if (useChecker) {
        checker = ComplexChecker{checker_};
    }
    return std::make_shared<DecisionTreeHeurCalculator>(
            heurMultiplier, calculator_,
            DecisionTreeHeurListFactory{
                *table_,
                heurList_ | boost::adaptors::transformed(
                        IncrementInfo::getHeurInfo),
                checker,
                maxDepth,
                options_.numThreads_});
}

void BlockListGenerator::dumpStatus(const Status& status, const Point *p, const std::string& title)
{
    const std::size_t* threadId = util::ThreadPool::getCurrentThreadId();
    std::ostream* dump = &dump_;
    if (threadId) {
        dump = &calculationInfos_[*threadId]->dump_;
    }
    if (p) {
        Array<bool> hl = status.reachableArray();
        hl[*p] = true;
        ::dumpStatus(*dump, status, title, &hl);
    } else {
        ::dumpStatus(*dump, status, title, &status.reachableArray());
    }
}
