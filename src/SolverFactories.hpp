#ifndef SOLVERFACTORIES_H_
#define SOLVERFACTORIES_H_

#include "Checker/ComplexCheckerFwd.hpp"

#include "ExpanderFactory.hpp"
#include "PrioNodeQueueFwd.hpp"

#include "util/TimeMeter.hpp"

class HeurCalculator;
struct Options;
class Dumper;
class Checker;
class NodeChecker;
class Table;

std::shared_ptr<PrioNodeQueue> createPrioQueueFromOptions(const Options& opts);
std::shared_ptr<Dumper> createDumperFromOptions(const Options& opts);


class OptionsBasedExpanderFactory {
    const Options& options_;
    const Table& table_;
    std::size_t* expandedNodes_;
    util::TimerData* chokePointFindingTime_;
    util::TimerData* preprocessingIterationTime_;
public:
    OptionsBasedExpanderFactory(const Options& opts, const Table& table,
            std::size_t* expandedNodes, util::TimerData* chokePointFindingTime,
            util::TimerData* preprocessingIterationTime):
        options_(opts),
        table_(table),
        expandedNodes_(expandedNodes),
        chokePointFindingTime_(chokePointFindingTime),
        preprocessingIterationTime_(preprocessingIterationTime)
    {
    }

    OptionsBasedExpanderFactory(const OptionsBasedExpanderFactory&) = delete;
    OptionsBasedExpanderFactory& operator=(const OptionsBasedExpanderFactory&) = delete;

    std::shared_ptr<const HeurCalculator> createAdvancedHeurCalcularor(float heurMultiplier);
    std::shared_ptr<Expander> createExpander(
            bool allowMultiThread,
            std::shared_ptr<const HeurCalculator> calculator,
            ComplexChecker checker,
            ComplexNodeChecker nodeChecker,
            std::size_t* expandedNodes,
            std::shared_ptr<const HeurCalculator> experimentalCalculator =
                    std::shared_ptr<const HeurCalculator>());
    std::vector<std::shared_ptr<const Checker>> createBasicCheckers(
            const std::shared_ptr<const HeurCalculator>& calculator);
    std::deque<std::shared_ptr<const NodeChecker>> createBasicNodeCheckers(
            const std::shared_ptr<const HeurCalculator>& calculator,
            const Status& status);

    ExpanderFactory factory();
};


#endif /* SOLVERFACTORIES_H_ */
