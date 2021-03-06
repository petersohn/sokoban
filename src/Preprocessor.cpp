#include "Checker/BlockListChecker.hpp"

#include "Dumper/DumperFunctions.hpp"

#include "Preprocessor.hpp"
#include "HeurCalculator/BlockListHeurCalculator.hpp"
#include "HeurCalculator/DecisionTreeHeurCalculator.hpp"
#include "HeurCalculator/TableHeurCalculator.hpp"
#include "HeurCalculator/HeurCalculator.hpp"

#include "ChokePointFinder.hpp"
#include "SaverThread.hpp"
#include "Solver.hpp"
#include "SubStatusForEach.hpp"

#include <boost/optional.hpp>
#include <boost/range/algorithm.hpp>

#include <algorithm>
#include <functional>
#include <iterator>
#include <iostream>
#include <cstdlib>

namespace sokoban {

Preprocessor::Preprocessor(std::unique_ptr<const Solver> solver,
        std::shared_ptr<const HeurCalculator> calculator, ComplexChecker checker,
        const Options& options, Saver saver):
    solver_(std::move(solver)),
    calculator_(std::move(calculator)),
    checker_(std::move(checker)),
    options_(options),
    saver_(saver),
    dump_("blocklist.dump"),
    threadPool_()
{
    threadPool_.setNumThreads(options.numThreads_);
}

Preprocessor::~Preprocessor()
{}

std::deque<std::shared_ptr<Node>> Preprocessor::calculateBlockList(
        const Status& status)
{
    std::deque<std::shared_ptr<Node>> result = solver_->solve(status);
    if (result.empty()) {
        std::size_t threadId = *util::ThreadPool::getCurrentThreadId();
        calculationInfos_[threadId]->blockList_.push_back(status);
        dumpStatus(status, nullptr, "Blocked");
    }
    return result;
}

void Preprocessor::calculateHeurList(const Status& status, std::size_t index)
{
    std::size_t threadId = *util::ThreadPool::getCurrentThreadId();
    auto& calculationInfo = calculationInfos_[threadId];

    ++calculationInfo->callNum_;

    std::deque<std::shared_ptr<Node>> result = calculateBlockList(status);
    if (!result.empty()) {
        float heur = incrementalCalculator_->calculateStatus(status);
        float cost = result.back()->cost();
        float difference = cost - heur;
        if (difference > 0) {
            calculationInfo->dump_ <<
                    heur << " --> " << cost << "(" << difference << ")\n";
            dumpStatus(status, nullptr, "Added heur");
            HeurInfo heurInfo{status, cost};
            calculationInfo->heurList_.push_back(
                    IncrementInfo{std::move(heurInfo), difference});
        }
    }

    calculationInfo->maxIndex_ = index;
}

void Preprocessor::init(const Table& table)
{
    table_ = &table;
    std::cerr << "Calculating block list..." << std::endl;
    subStatusForEach_ = std::make_unique<SubStatusForEach>(table,
            [this](const Status& status, std::size_t index) {
                calculateHeurList(status, index);
            },
            SubStatusForEach::MinDistance{0},
            SubStatusForEach::MaxDistance{options_.blockListDistance_},
            SubStatusForEach::ChokePointDistantNum{options_.chokePointNum_ > 0 ?
                    options_.chokePointDistantNum_ : 0},
            calculateChokePoints(),
            SubStatusForEach::WorkQueueLength{options_.workQueueLength_},
            SubStatusForEach::ReverseSearchMaxDepth{
                    options_.reverseSearchMaxDepth_},
            threadPool_.getIoService());

    blockList_.clear();
    heurList_.clear();
    calculationInfos_.resize(options_.numThreads_);

    currentStoneNum_ = 2;
    maxIndex_ = 0;
}

void Preprocessor::run() {
    std::size_t decisionTreeDepth =
            options_.blocklistHeurCalculatorType_ ==
                    BlockListHeurType::decisionTree ?
            options_.blocklistDecisionTreeDepth_ : 0;

    std::unique_ptr<SaverThread> saverThread;
    if (!options_.saveProgress_.empty() && options_.saveInterval_ > 0) {
        saverThread = std::make_unique<SaverThread>(
                [this]() {
                        subStatusForEach_->synchronize( [this]() { save(); });
                },
                boost::posix_time::millisec(
                        static_cast<int>(options_.saveInterval_ * 1000.0f)));

    }

    util::TimeMeter timeMeter;
    if (!options_.preprocessFillBeforeCalculate_) {
        threadPool_.start();
    }

    for (; currentStoneNum_ <= options_.blockListStones_; ++currentStoneNum_) {
        incrementalCalculator_ = currentStoneNum_ == 2 ?
            calculator_ : decisionTreeDepth > 0 ?
                decisionTreeHeurCalculator(decisionTreeDepth, false, 1.0f) :
                vectorHeurCalculator(1.0f);

        for (auto& calculationInfo: calculationInfos_) {
            calculationInfo = std::make_unique<CalculationInfo>();
        }

        std::cerr << "Stones = " << currentStoneNum_ << std::endl;

        ComplexChecker actualChecker{checker_};
        actualChecker.append(checker());
        subStatusForEach_->start(currentStoneNum_, calculator_, actualChecker,
                Matrix<bool>{table_->width(), table_->height(), false},
                maxIndex_);
        std::cerr << "Waiting for processing to finish..." << std::endl;

        if (saverThread) {
            saverThread->start();
        }

        if (options_.preprocessFillBeforeCalculate_) {
            threadPool_.start();
        }

        subStatusForEach_->wait(true);

        if (options_.preprocessFillBeforeCalculate_) {
            threadPool_.wait();
        }

        if (saverThread) {
            saverThread->stop();
        }

        updateResult();
    }

    if (!options_.preprocessFillBeforeCalculate_) {
        threadPool_.wait();
    }

    iteratingTime_ = timeMeter.data();
    if (options_.maxHeurListSize_ > 0 &&
            heurList_.size() > options_.maxHeurListSize_) {
        IncrementList(heurList_.begin(),
                heurList_.begin() + options_.maxHeurListSize_).swap(heurList_);
    }

    std::cerr << "Heur list size = " << heurList_.size() << "\n" <<
            "Processor time spent on saving: " << savingTime_.processorTime <<
            "\nReal time spent on saving: " << savingTime_.realTime << "\n";
    dump_.flush();
}

void Preprocessor::save()
{
    util::TimeMeter timeMeter;

    aggregateThreadResults();

    maxIndex_ = (*boost::max_element(calculationInfos_,
            [](const auto& lhs, const auto& rhs) {
                return lhs->maxIndex_ < rhs->maxIndex_;
            }))->maxIndex_ + 1;

    saver_(*this);
    savingTime_.processorTime += timeMeter.processorTime();
    savingTime_.realTime += timeMeter.realTime();
}

Matrix<bool> Preprocessor::calculateChokePoints()
{
    Matrix<bool> result;

    util::TimeMeter timeMeter;
    if (options_.chokePointNum_ > 0) {
        result = findChokePoints(*table_, options_, calculator_,
                ComplexChecker{checker_}, true);
        chokePointFinderTime_ = timeMeter.data();
        Status chokePointStatus{*table_};

        for (Point p: matrixRange(result)) {
            if (result[p]) {
                chokePointStatus.addStone(p);
            }
        }

        sokoban::dumpStatus(std::cerr, chokePointStatus, "Choke points");
    }

    return result;
}

void Preprocessor::updateResult()
{
    std::size_t callNum = aggregateThreadResults();
    maxIndex_ = 0;

    std::cerr << "Block list size = " << blockList_.size() << std::endl;
    std::cerr << "Heur list size = " << heurList_.size() << std::endl;
    std::cerr << "Calculations made = " << callNum << "\n";
}

std::size_t Preprocessor::aggregateThreadResults()
{
    std::size_t callNum = 0;
    for (const auto& calculationInfo: calculationInfos_) {
        callNum += calculationInfo->callNum_;
        dump_ << calculationInfo->dump_.str();

        for (const auto& status: calculationInfo->blockList_) {
            blockList_.add(status);
        }

        heurList_.reserve(heurList_.size() +
                calculationInfo->heurList_.size());
        std::move(calculationInfo->heurList_.begin(),
                calculationInfo->heurList_.end(),
                std::back_inserter(heurList_));

        calculationInfo->blockList_.clear();
        calculationInfo->heurList_.clear();
    }

    boost::sort(heurList_,
            [](const IncrementInfo& left, const IncrementInfo& right)
            {
                return left.difference_ > right.difference_ ||
                        (left.difference_ == right.difference_ &&
                        left.heurInfo_.first.state().size() <
                        right.heurInfo_.first.state().size());
            });

    return callNum;
}

std::shared_ptr<Checker> Preprocessor::checker()
{
    assert(table_);
    return std::make_shared<BlockListChecker>(blockList_);
}

std::shared_ptr<HeurCalculator> Preprocessor::vectorHeurCalculator(float heurMultiplier)
{
    assert(table_);
    return std::make_shared<BlockListHeurCalculator>(
            heurMultiplier, calculator_,
            BlockListHeurListFactory{heurList_ | boost::adaptors::transformed(
                    IncrementInfo::getHeurInfo) | boost::adaptors::reversed});
}


std::shared_ptr<HeurCalculator> Preprocessor::decisionTreeHeurCalculator(
        std::size_t maxDepth, bool useChecker, float heurMultiplier)
{
    assert(table_);

    boost::optional<ComplexChecker> checker;
    if (useChecker) {
        checker = checker_;
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

void Preprocessor::dumpStatus(const Status& status, const Point *p,
        const std::string& title)
{
    const std::size_t* threadId = util::ThreadPool::getCurrentThreadId();
    std::ostream* dump = &dump_;
    if (threadId) {
        dump = &calculationInfos_[*threadId]->dump_;
    }
    Matrix<Highlight> hl = status.reachableMatrix();
    if (p) {
        hl[*p] = true;
    }
    sokoban::dumpStatus(*dump, status, title, hl);
}

} // namespace sokoban
