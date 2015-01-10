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
#include "HeurCalculator.hpp"
#include "ComplexChecker.hpp"
#include <vector>
#include <functional>

namespace {

std::shared_ptr<PrioNodeQueue> createPrioQueue()
{
	std::vector<Compare> compares{{CompareMethod::depth, true}};
	return std::make_shared<PrioNodeQueue>(CompareQueue{std::move(compares)});
}


}

std::shared_ptr<PrioNodeQueue> createPrioQueueFromOptions(const Options& opts)
{
	return std::make_shared<PrioNodeQueue>(CompareQueue{opts.compare_});
}

std::shared_ptr<const HeurCalculator> OptionsBasedExpanderFactory::createAdvancedHeurCalcularor(
		float heurMultiplier)
{
	auto basicHeurCalculator = std::make_shared<BasicHeurCalculator>(
			BasicStoneCalculator{table_}, 1.0f);
	std::unique_ptr<const Solver> solver(new Solver(createPrioQueue,
		[this, basicHeurCalculator]() {
			return createExpander(
					basicHeurCalculator,
					ComplexChecker{createBasicCheckers(basicHeurCalculator)},
					nullptr);
		}));
	return std::make_shared<AdvancedHeurCalculator>(AdvancedStoneCalculator{
			table_, std::move(solver), options_.reverseSearchMaxDepth_,
			options_.partitionsDumpFilename_}, heurMultiplier);
}

std::vector<std::shared_ptr<const Checker>> OptionsBasedExpanderFactory::createBasicCheckers(const std::shared_ptr<const HeurCalculator>& calculator)
{
	std::vector<std::shared_ptr<const Checker>> checkers;
	switch (options_.movableCheckerType_) {
	case MovableCheckerType::none:
		break;
	case MovableCheckerType::simple:
		checkers.push_back(std::shared_ptr<const Checker>(new MovableChecker(calculator)));
		break;
	case MovableCheckerType::extended:
		checkers.push_back(std::shared_ptr<const Checker>(new ExtendedMovableChecker(calculator)));
		break;
	}

	switch (options_.corridorCheckerType_) {
	case CorridorCheckerType::none:
		break;
	case CorridorCheckerType::simple:
		checkers.push_back(std::shared_ptr<const Checker>(new CorridorChecker(
						CorridorCheckerStrategyFactory{calculator})));
		break;
	case CorridorCheckerType::extended:
		checkers.push_back(std::shared_ptr<const Checker>(new ExtendedCorridorChecker(
						ExtendedCorridorCheckerStrategyFactory{calculator})));
		break;
	}

	return checkers;
}

std::shared_ptr<Expander> OptionsBasedExpanderFactory::createExpander(
			std::shared_ptr<const HeurCalculator> calculator,
			ComplexChecker checker,
			std::size_t* expandedNodes,
			std::shared_ptr<const HeurCalculator> experimentalCalculator)
{
	auto visitedStates = std::make_shared<VisitedStates>();
	std::shared_ptr<NodeFactory> nodeFactory(new NodeFactory(calculator,
				experimentalCalculator));
	std::shared_ptr<Expander> expander = std::make_shared<NormalExpander>(visitedStates,
			calculator, std::move(checker), nodeFactory, expandedNodes);

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
	std::vector<std::shared_ptr<const Checker>> checkers = createBasicCheckers(calculator);
	if (options_.blockListStones_ > 1) {
		ComplexChecker checker{checkers};
		std::shared_ptr<const HeurCalculator> preprocessingCalculator =
				options_.useAdvancedHeurCalculator_ ?
				createAdvancedHeurCalcularor(1.0f) :
				std::make_shared<BasicHeurCalculator>(BasicStoneCalculator{table_}, 1.0f);
		std::unique_ptr<const Solver> solver(new Solver(
				std::bind(&createPrioQueueFromOptions, options_),
				[this, preprocessingCalculator, checker]() {
					return createExpander(preprocessingCalculator, checker,
							nullptr);
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
			calculator, ComplexChecker{checkers}, expandedNodes_, experimentalCalculator);
}

namespace {

std::string getDumpFilename(const Options& options, const std::string& defaultValue)
{
	return options.dumpFilename_.empty() ? defaultValue : options.dumpFilename_;
}

}

std::shared_ptr<Dumper> createDumperFromOptions(const Options & opts)
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

		return std::shared_ptr<Dumper>(new TextDumper(getDumpFilename(opts, "dump.dump"), dumpFilter));
	}
	case DumpStyle::xml:
		return std::shared_ptr<Dumper>(new XDumper(getDumpFilename(opts, "dump.xml")));
	case DumpStyle::statistics:
		return std::shared_ptr<Dumper>(new StatisticsDumper(getDumpFilename(opts, "dump.csv")));
	default:
		return std::shared_ptr<Dumper>();
	}
}





