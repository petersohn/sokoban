#include "SolverFactories.hpp"
#include "Solver.hpp"
#include "PrioNodeQueue.hpp"
#include "NormalExpander.hpp"
#include "StonePusher.hpp"
#include "MovableChecker.hpp"
#include "CorridorChecker.hpp"
#include "BlockListGenerator.hpp"
#include "BasicHeurCalculator.hpp"
#include "AdvancedHeurCalculator.hpp"
#include "VisitedStates.hpp"
#include "CompareQueue.hpp"
#include "Dumper/TextDumper.hpp"
#include "Dumper/XDumper.hpp"
#include "Dumper/StatisticsDumper.hpp"
#include "NodeFactory.hpp"
#include <vector>
#include <functional>

namespace {

std::shared_ptr<PrioNodeQueue> createPrioQueue()
{
	std::vector<Compare> compares{{CompareMethod::depth, true}};
	return std::make_shared<PrioNodeQueue>(CompareQueue{std::move(compares)});
}


}

std::shared_ptr<PrioNodeQueue> createPrioQueueFromOptions(const Options &opts)
{
	return std::make_shared<PrioNodeQueue>(CompareQueue{opts.compare_});
}

HeurCalculator::Ptr OptionsBasedExpanderFactory::createAdvancedHeurCalcularor()
{
	auto basicHeurCalculator = std::make_shared<BasicHeurCalculator>(
			BasicStoneCalculator{table_});
	Solver::Ptr solver(new Solver(createPrioQueue,
		[this, basicHeurCalculator]() {
			return createExpander(
					basicHeurCalculator,
					ComplexChecker{createBasicCheckers(basicHeurCalculator)},
					false);
		}));
	return std::make_shared<AdvancedHeurCalculator>(AdvancedStoneCalculator{
			table_, std::move(solver), options_.reverseSearchMaxDepth_});
}

std::vector<Checker::Ptr> OptionsBasedExpanderFactory::createBasicCheckers(const HeurCalculator::Ptr& calculator)
{
	std::vector<Checker::Ptr> checkers;
	if (options_.useMovableChecker_)
		checkers.push_back(Checker::Ptr(new MovableChecker(calculator)));
	if (options_.useCorridorChecker_)
		checkers.push_back(Checker::Ptr(new CorridorChecker(calculator)));
	return checkers;
}

Expander::Ptr OptionsBasedExpanderFactory::createExpander(
			HeurCalculator::Ptr calculator,
			ComplexChecker checker,
			bool log,
			HeurCalculator::Ptr experimentalCalculator)
{
	auto visitedStates = std::make_shared<VisitedStates>();
	NodeFactory::Ptr nodeFactory(new NodeFactory(calculator, experimentalCalculator));
	Expander::Ptr expander = std::make_shared<NormalExpander>(visitedStates,
			calculator, std::move(checker), nodeFactory, log);

	if (options_.useStonePusher_) {
		expander = std::make_shared<StonePusher>(expander, visitedStates,
				calculator, nodeFactory);
	}

	return expander;
}

ExpanderFactory OptionsBasedExpanderFactory::factory()
{
	HeurCalculator::Ptr calculator =
		options_.useAdvancedHeurCalculator_ ?
		createAdvancedHeurCalcularor() :
		std::make_shared<BasicHeurCalculator>(BasicStoneCalculator{table_});
	HeurCalculator::Ptr experimentalCalculator;
	std::vector<Checker::Ptr> checkers = createBasicCheckers(calculator);
	if (options_.blockListStones_ > 1) {
		ComplexChecker checker{checkers};
		Solver::Ptr solver(new Solver(
				std::bind(&createPrioQueueFromOptions, options_),
				[this, calculator, checker]() {
					return createExpander(calculator, checker, false);
				})) ;
		BlockListGenerator blockListGenerator(
				std::move(solver), calculator, checker, options_);
		blockListGenerator.init(table_);
		checkers.push_back(blockListGenerator.checker());
		switch (options_.blocklistHeurCalculatorType_) {
		case BlockListHeurType::none:
			break;
		case BlockListHeurType::vector:
			calculator = blockListGenerator.vectorHeurCalculator();
//			experimentalCalculator = blockListGenerator.vectorHeurCalculator2();
			break;
		case BlockListHeurType::decisionTree:
			calculator = blockListGenerator.decisionTreeHeurCalculator(
					options_.maxDecisionTreeDepth_,
					options_.useCheckerForDecisionTree_);
			break;
		}
	}
//	experimentalCalculator = calculator;
	return std::bind(&OptionsBasedExpanderFactory::createExpander, this,
			calculator,
			ComplexChecker{checkers},
			log_,
			experimentalCalculator);
}

Dumper::Ptr createDumperFromOptions(const Options & opts)
{
	switch (opts.dumpStyle_) {
	case DumpStyle::text:
		return Dumper::Ptr(new TextDumper("dump.dump"));
	case DumpStyle::xml:
		return Dumper::Ptr(new XDumper("dump.xml"));
	case DumpStyle::statistics:
		return Dumper::Ptr(new StatisticsDumper("dump.csv"));
	default:
		return Dumper::Ptr();
	}
}





