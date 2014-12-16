#include "SolverFactories.hpp"
#include "Solver.hpp"
#include "PrioNodeQueue.hpp"
#include "NormalExpander.hpp"
#include "StonePusher.hpp"
#include "MovableChecker.hpp"
#include "ExtendedMovableChecker.hpp"
#include "CorridorChecker.hpp"
#include "ExtendedCorridorChecker.hpp"
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

std::shared_ptr<const HeurCalculator> OptionsBasedExpanderFactory::createAdvancedHeurCalcularor(
		float heurMultiplier)
{
	auto basicHeurCalculator = std::make_shared<BasicHeurCalculator>(
			BasicStoneCalculator{table_}, 1.0f);
	Solver::Ptr solver(new Solver(createPrioQueue,
		[this, basicHeurCalculator]() {
			return createExpander(
					basicHeurCalculator,
					ComplexChecker{createBasicCheckers(basicHeurCalculator)},
					false);
		}));
	return std::make_shared<AdvancedHeurCalculator>(AdvancedStoneCalculator{
			table_, std::move(solver), options_.reverseSearchMaxDepth_,
			options_.partitionsDumpFilename_}, heurMultiplier);
}

std::vector<Checker::Ptr> OptionsBasedExpanderFactory::createBasicCheckers(const std::shared_ptr<const HeurCalculator>& calculator)
{
	std::vector<Checker::Ptr> checkers;
	switch (options_.movableCheckerType_) {
	case MovableCheckerType::none:
		break;
	case MovableCheckerType::simple:
		checkers.push_back(Checker::Ptr(new MovableChecker(calculator)));
		break;
	case MovableCheckerType::extended:
		checkers.push_back(Checker::Ptr(new ExtendedMovableChecker(calculator)));
		break;
	}

	switch (options_.corridorCheckerType_) {
	case CorridorCheckerType::none:
		break;
	case CorridorCheckerType::simple:
		checkers.push_back(Checker::Ptr(new CorridorChecker(
						CorridorCheckerStrategyFactory{calculator})));
		break;
	case CorridorCheckerType::extended:
		checkers.push_back(Checker::Ptr(new ExtendedCorridorChecker(
						ExtendedCorridorCheckerStrategyFactory{calculator})));
		break;
	}

	return checkers;
}

Expander::Ptr OptionsBasedExpanderFactory::createExpander(
			std::shared_ptr<const HeurCalculator> calculator,
			ComplexChecker checker,
			bool log,
			std::shared_ptr<const HeurCalculator> experimentalCalculator)
{
	auto visitedStates = std::make_shared<VisitedStates>();
	NodeFactory::Ptr nodeFactory(new NodeFactory(calculator,
				experimentalCalculator));
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
	std::shared_ptr<const HeurCalculator> calculator =
		options_.useAdvancedHeurCalculator_ ?
		createAdvancedHeurCalcularor(options_.heurMultiplier_) :
		std::make_shared<BasicHeurCalculator>(BasicStoneCalculator{table_},
				options_.heurMultiplier_);
	std::shared_ptr<const HeurCalculator> experimentalCalculator;
	std::vector<Checker::Ptr> checkers = createBasicCheckers(calculator);
	if (options_.blockListStones_ > 1) {
		ComplexChecker checker{checkers};
		std::shared_ptr<const HeurCalculator> preprocessingCalculator =
				options_.useAdvancedHeurCalculator_ ?
				createAdvancedHeurCalcularor(1.0f) :
				std::make_shared<BasicHeurCalculator>(BasicStoneCalculator{table_}, 1.0f);
		Solver::Ptr solver(new Solver(
				std::bind(&createPrioQueueFromOptions, options_),
				[this, preprocessingCalculator, checker]() {
					return createExpander(preprocessingCalculator, checker, false);
				})) ;
		BlockListGenerator blockListGenerator(
				std::move(solver), preprocessingCalculator, checker, options_);
		blockListGenerator.init(table_);
		checkers.push_back(blockListGenerator.checker());
		switch (options_.blocklistHeurCalculatorType_) {
		case BlockListHeurType::none:
			break;
		case BlockListHeurType::vector:
			calculator = blockListGenerator.vectorHeurCalculator(options_.heurMultiplier_);
//			experimentalCalculator = blockListGenerator.vectorHeurCalculator2();
			break;
		case BlockListHeurType::decisionTree:
			calculator = blockListGenerator.decisionTreeHeurCalculator(
					options_.maxDecisionTreeDepth_,
					options_.useCheckerForDecisionTree_,
					options_.heurMultiplier_);
			break;
		}
	}
//	experimentalCalculator = calculator;
	return std::bind(&OptionsBasedExpanderFactory::createExpander, this,
			calculator, ComplexChecker{checkers}, log_, experimentalCalculator);
}

namespace {

std::string getDumpFilename(const Options& options, const std::string& defaultValue)
{
	return options.dumpFilename_.empty() ? defaultValue : options.dumpFilename_;
}

}

Dumper::Ptr createDumperFromOptions(const Options & opts)
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

		return Dumper::Ptr(new TextDumper(getDumpFilename(opts, "dump.dump"), dumpFilter));
	}
	case DumpStyle::xml:
		return Dumper::Ptr(new XDumper(getDumpFilename(opts, "dump.xml")));
	case DumpStyle::statistics:
		return Dumper::Ptr(new StatisticsDumper(getDumpFilename(opts, "dump.csv")));
	default:
		return Dumper::Ptr();
	}
}





