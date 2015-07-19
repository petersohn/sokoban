#ifndef SOLVERFACTORIES_H_
#define SOLVERFACTORIES_H_

#include "Checker/ComplexCheckerFwd.hpp"

#include "ExpanderFactory.hpp"
#include "PrioNodeQueueFwd.hpp"

#include "util/TimeMeter.hpp"

#include <boost/serialization/type_info_implementation.hpp>
#include <boost/serialization/shared_ptr.hpp>

class BlockListGenerator;
class Checker;
class Dumper;
class HeurCalculator;
class NodeChecker;
struct Options;
struct PreprocessedResult;
class Table;

std::shared_ptr<PrioNodeQueue> createPrioQueueFromOptions(const Options& opts);
std::shared_ptr<Dumper> createDumperFromOptions(const Options& opts);

class OptionsBasedExpanderFactory {
    using Checkers = std::vector<std::shared_ptr<const Checker>>;
    using NodeCheckers = std::deque<std::shared_ptr<NodeChecker>>;
    const Options& options_;
    const Table& table_;
    std::function<void(const BlockListGenerator&)> preprocessSaver;
    std::size_t* expandedNodes_;
    util::TimerData* chokePointFindingTime_;
    util::TimerData* preprocessingIterationTime_;

    std::shared_ptr<const HeurCalculator> createHeurCalculator(
            float heurMultiplier);
public:
    OptionsBasedExpanderFactory(const Options& opts, const Table& table,
            std::function<void(const BlockListGenerator&)> preprocessSaver,
            std::size_t* expandedNodes, util::TimerData* chokePointFindingTime,
            util::TimerData* preprocessingIterationTime):
        options_(opts),
        table_(table),
        preprocessSaver(preprocessSaver),
        expandedNodes_(expandedNodes),
        chokePointFindingTime_(chokePointFindingTime),
        preprocessingIterationTime_(preprocessingIterationTime)
    {
    }

    OptionsBasedExpanderFactory(const OptionsBasedExpanderFactory&) = delete;
    OptionsBasedExpanderFactory& operator=(
            const OptionsBasedExpanderFactory&) = delete;

    std::shared_ptr<const HeurCalculator> createAdvancedHeurCalcularor(
            float heurMultiplier);

    std::shared_ptr<Expander> createExpander(
            bool allowMultiThread,
            std::shared_ptr<const HeurCalculator> calculator,
            ComplexChecker checker,
            ComplexNodeChecker nodeChecker,
            std::size_t* expandedNodes,
            std::shared_ptr<const HeurCalculator> experimentalCalculator =
                    std::shared_ptr<const HeurCalculator>());

    Checkers createBasicCheckers(
            const std::shared_ptr<const HeurCalculator>& calculator);

    NodeCheckers createBasicNodeCheckers(
            const std::shared_ptr<const HeurCalculator>& calculator,
            const Status& status);

    std::unique_ptr<BlockListGenerator> createBlockListGenerator();
    PreprocessedResult preprocess(BlockListGenerator& blockListGenerator);

    ExpanderFactory factory(const PreprocessedResult& preprocessedResult);

    void setHeurCalculatorParameters(HeurCalculator& heurCalculator);
};


#endif /* SOLVERFACTORIES_H_ */
