#include "SolverFactories.hpp"

#include "Checker/BlockListChecker.hpp"
#include "Checker/ComplexCheckerBase.hpp"
#include "Checker/CorridorChecker.hpp"
#include "Checker/DistanceChecker.hpp"
#include "Checker/ExtendedCorridorChecker.hpp"
#include "Checker/ExtendedMovableChecker.hpp"
#include "Checker/MovableChecker.hpp"
#include "Checker/VisitedStates.hpp"
#include "Checker/VisitedStatesChecker.hpp"

#include "Dumper/BestDumper.hpp"
#include "Dumper/StatisticsDumper.hpp"
#include "Dumper/TextDumper.hpp"
#include "Dumper/XDumper.hpp"

#include "HeurCalculator/AdvancedHeurCalculator.hpp"
#include "HeurCalculator/BasicHeurCalculator.hpp"
#include "HeurCalculator/BlockListHeurCalculator.hpp"
#include "HeurCalculator/DecisionTreeHeurCalculator.hpp"
#include "HeurCalculator/HeurCalculator.hpp"

#include "Preprocessor.hpp"
#include "CompareQueue.hpp"
#include "DynamicVisitor.hpp"
#include "ExpandedNodeLimiter.hpp"
#include "MultiThreadExpander.hpp"
#include "NodeFactory.hpp"
#include "NormalExpander.hpp"
#include "PreprocessedResult.hpp"
#include "PrioNodeQueue.hpp"
#include "Solver.hpp"
#include "StonePusher.hpp"

#include <boost/mpl/vector.hpp>

#include <functional>
#include <vector>

namespace {

PrioNodeQueue createPrioQueue()
{
    std::vector<Compare> compares{{CompareMethod::depth, true}};
    return PrioNodeQueue{CompareQueue{std::move(compares)}};
}

} // unnamed namespace

PrioNodeQueue createPrioQueueFromOptions(const Options& options)
{
    return PrioNodeQueue{CompareQueue{options.compare_}};
}

std::shared_ptr<const HeurCalculator> OptionsBasedExpanderFactory::createAdvancedHeurCalcularor(
        float heurMultiplier)
{
    auto basicHeurCalculator = std::make_shared<BasicHeurCalculator>(
            BasicStoneCalculator{table_}, 1.0f);
    auto solver = std::make_unique<const Solver>(createPrioQueue,
        [this, basicHeurCalculator](const Status& status) {
            return createExpander(
                    false,
                    basicHeurCalculator,
                    ComplexChecker{createBasicCheckers(basicHeurCalculator)},
                    ComplexNodeChecker{createBasicNodeCheckers(
                            basicHeurCalculator, status)},
                    nullptr);
        });
    return std::make_shared<AdvancedHeurCalculator>(AdvancedStoneCalculator{
            table_, std::move(solver), options_.reverseSearchMaxDepth_,
            options_.partitionsDumpFilename_}, heurMultiplier);
}

auto OptionsBasedExpanderFactory::createBasicCheckers(
        const std::shared_ptr<const HeurCalculator>& calculator) -> Checkers
{
    std::vector<std::shared_ptr<const Checker>> checkers;
    switch (options_.movableCheckerType_) {
    case MovableCheckerType::none:
        break;
    case MovableCheckerType::simple:
        checkers.push_back(std::make_shared<MovableChecker>(calculator));
        break;
    case MovableCheckerType::extended:
        checkers.push_back(std::make_shared<ExtendedMovableChecker>(calculator));
        break;
    }

    switch (options_.corridorCheckerType_) {
    case CorridorCheckerType::none:
        break;
    case CorridorCheckerType::simple:
        checkers.push_back(std::make_shared<CorridorChecker>(
                        CorridorCheckerStrategyFactory{calculator}));
        break;
    case CorridorCheckerType::extended:
        checkers.push_back(std::make_shared<ExtendedCorridorChecker>(
                        ExtendedCorridorCheckerStrategyFactory{calculator}));
        break;
    }

    return checkers;
}

auto OptionsBasedExpanderFactory::createBasicNodeCheckers(
        const std::shared_ptr<const HeurCalculator>& calculator,
        const Status& status) -> NodeCheckers
{
    VisitedStates visitedStates;
    visitedStates.checkAndPush(std::pair<const Status&, int>(status,
            calculator->calculateStatus(status)));

    return {std::make_shared<VisitedStatesChecker>(visitedStates)};
}

std::unique_ptr<Expander> OptionsBasedExpanderFactory::createExpander(
            bool allowMultiThread,
            std::shared_ptr<const HeurCalculator> calculator,
            ComplexChecker checker,
            ComplexNodeChecker nodeChecker,
            std::size_t* expandedNodes,
            std::shared_ptr<const HeurCalculator> experimentalCalculator)
{
    auto nodeFactory = std::make_shared<NodeFactory>(calculator,
                experimentalCalculator);
    std::unique_ptr<Expander> expander;
    if (allowMultiThread && options_.numThreads_ > 1 && options_.parallelExpand_) {
        expander = std::make_unique<MultiThreadExpander>(
                calculator, std::move(checker), nodeChecker, nodeFactory,
                expandedNodes, options_.numThreads_);
    } else {
        expander = std::make_unique<NormalExpander>(
                calculator, std::move(checker), nodeChecker, nodeFactory,
                expandedNodes);
    }

    if (options_.useStonePusher_) {
        return std::make_unique<StonePusher>(std::move(expander),
                std::move(nodeChecker),
                calculator, nodeFactory);
    }

    return expander;
}

std::shared_ptr<const HeurCalculator>
OptionsBasedExpanderFactory::createHeurCalculator(float heurMultiplier)
{
    return options_.useAdvancedHeurCalculator_ ?
            createAdvancedHeurCalcularor(heurMultiplier) :
            std::make_shared<BasicHeurCalculator>(BasicStoneCalculator{table_},
                    heurMultiplier);
}

ExpanderFactory OptionsBasedExpanderFactory::factory(
        const PreprocessedResult& preprocessedResult)
{
    std::shared_ptr<const HeurCalculator> calculator;

    if (preprocessedResult.heurCalculator) {
        calculator = preprocessedResult.heurCalculator;
    } else {
        calculator = createHeurCalculator(options_.heurMultiplier_);
    }

    auto checkers = createBasicCheckers(calculator);

    if (preprocessedResult.checker) {
        checkers.push_back(preprocessedResult.checker);
    }

    std::shared_ptr<const HeurCalculator> experimentalCalculator;
//    experimentalCalculator = calculator;
    return [=](const Status& status) {
            auto nodeCheckers = createBasicNodeCheckers(calculator, status);
            if (options_.distanceLimiter_) {
                auto basicHeurCalculator = std::make_shared<BasicHeurCalculator>(
                        BasicStoneCalculator{table_}, 1.0f);
                nodeCheckers.push_front(std::make_shared<DistanceChecker>(
                        basicHeurCalculator));
            }

            if (options_.expandedNodeLimit_ > 0) {
                assert(expandedNodes_);
                nodeCheckers.push_front(std::make_shared<ExpandedNodeLimiter>(
                        *expandedNodes_, options_.expandedNodeLimit_));
            }

            return createExpander(true, calculator, ComplexChecker{checkers},
                    ComplexNodeChecker{nodeCheckers}, expandedNodes_,
                    experimentalCalculator);
        };
}

std::unique_ptr<Preprocessor> OptionsBasedExpanderFactory::createPreprocessor()
{
    std::shared_ptr<const HeurCalculator> preprocessingCalculator =
            createHeurCalculator(1.0f);

    ComplexChecker checker{createBasicCheckers(preprocessingCalculator)};
    auto solver = std::make_unique<Solver>(
            std::bind(&createPrioQueueFromOptions, options_),
            [=](const Status& status) {
                return createExpander(false, preprocessingCalculator, checker,
                        ComplexNodeChecker{createBasicNodeCheckers(
                                preprocessingCalculator, status)},
                        nullptr);
            });

    auto preprocessor = std::make_unique<Preprocessor>(
            std::move(solver), preprocessingCalculator, checker,
            options_, preprocessSaver);
    preprocessor->init(table_);
    return preprocessor;
}

PreprocessedResult OptionsBasedExpanderFactory::preprocess(
        Preprocessor& preprocessor)
{
    preprocessor.run();

    if (chokePointFindingTime_) {
        *chokePointFindingTime_ = preprocessor.chokePointFinderTime();
    }
    if (preprocessingIterationTime_) {
        *preprocessingIterationTime_ = preprocessor.iteratingTime();
    }

    PreprocessedResult result;
    result.checker = preprocessor.checker();

    switch (options_.blocklistHeurCalculatorType_) {
    case BlockListHeurType::none:
        break;
    case BlockListHeurType::vector:
         result.heurCalculator =
                 preprocessor.vectorHeurCalculator(options_.heurMultiplier_);
        break;
    case BlockListHeurType::decisionTree:
        result.heurCalculator = preprocessor.decisionTreeHeurCalculator(
                options_.maxDecisionTreeDepth_,
                options_.useCheckerForDecisionTree_,
                options_.heurMultiplier_);
        break;
    }

    return result;
}

void OptionsBasedExpanderFactory::setHeurCalculatorParameters(
        HeurCalculator& heurCalculator)
{
    applyDynamicVisitor<boost::mpl::vector<
            BlockListHeurCalculator, DecisionTreeHeurCalculator>>(
            [this](auto& heurCalculator) {
                heurCalculator.setBaseCalculator(createHeurCalculator(
                        options_.heurMultiplier_));
                heurCalculator.setHeurMultiplier(options_.heurMultiplier_);
            },
            heurCalculator);
}

namespace {

std::string getDumpFilename(const Options& options, const std::string& defaultValue)
{
    return options.dumpFilename_.empty() ? defaultValue : options.dumpFilename_;
}

}

std::unique_ptr<Dumper> createDumperFromOptions(const Options & opts)
{
    switch (opts.dumpStyle_) {
    case DumpStyle::text: {
        TextDumper::FilterType dumpFilter;

        switch (opts.dumpFilterType_) {
        case DumpFilterType::none:
            dumpFilter = TextDumper::NoFilter{};
            break;
        case DumpFilterType::text:
            dumpFilter = opts.dumpFilter_;
            break;
        case DumpFilterType::regex:
            dumpFilter = std::regex{opts.dumpFilter_};
            break;
        }

        return std::make_unique<TextDumper>(
                getDumpFilename(opts, "dump.dump"), dumpFilter);
    }
    case DumpStyle::xml:
        return std::make_unique<XDumper>(
                getDumpFilename(opts, "dump.xml"));
    case DumpStyle::statistics:
        return std::make_unique<StatisticsDumper>(
                getDumpFilename(opts, "dump.csv"));
    case DumpStyle::best:
        return std::make_unique<BestDumper>(getDumpFilename(opts, "dump.dump"));
    default:
        return {};
    }
}





