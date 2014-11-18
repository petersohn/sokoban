#include "SolverFactories.hpp"
#include "Solver.hpp"
#include "PrioNodeQueue.hpp"
#include "NormalExpander.hpp"
#include "StonePusher.hpp"
#include "ComplexStrategy.hpp"
#include "ComplexChecker.hpp"
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

NodeQueue::Ptr createPrioQueue()
{
	std::vector<CompareQueue<Node::Ptr>::FuncType> funcs;
	funcs.push_back(CompareByMethodPtr<Node::Ptr>(&Node::costFgv, false));
	funcs.push_back(CompareByMethodPtr<Node::Ptr>(&Node::depth, true));
	return NodeQueue::Ptr(new PrioNodeQueue<CompareQueue<Node::Ptr> >(CompareQueue<Node::Ptr>(
			funcs.begin(), funcs.end())));
}


}

NodeQueue::Ptr createPrioQueueFromOptions(const Options &opts)
{
	std::vector<CompareQueue<Node::Ptr>::FuncType> funcs;
	funcs.push_back([](const Node::Ptr& lhs, const Node::Ptr& rhs)
			{
				if (lhs->heur() == 0 && rhs->heur() != 0) {
					return -1;
				}

				if (rhs->heur() == 0 && lhs->heur() != 0) {
					return 1;
				}

				return 0;
			});
	funcs.push_back(CompareByMethodPtr<Node::Ptr>(&Node::costFgv, false));
	for (const auto& compareMethod: opts.compare_) {
		CompareByMethodPtr<Node::Ptr>::Fun fun;
		switch (compareMethod.type) {
		case CompareMethod::heur: fun = &Node::heur; break;
		case CompareMethod::depth: fun = &Node::depth; break;
		case CompareMethod::time: fun = &Node::time; break;
		}
		if (fun)
			funcs.push_back(CompareByMethodPtr<Node::Ptr>(fun,
						compareMethod.reverse));
	}
	return NodeQueue::Ptr(new PrioNodeQueue<CompareQueue<Node::Ptr> >(CompareQueue<Node::Ptr>(
			funcs.begin(), funcs.end())));
}

HeurCalculator::Ptr OptionsBasedExpanderFactory::createAdvancedHeurCalcularor()
{
	auto basicHeurCalculator = std::make_shared<BasicHeurCalculator>(
			BasicStoneCalculator{table_});
	Solver::Ptr solver(new Solver(createPrioQueue,
		[this, basicHeurCalculator]() {
			return createExpander(
					basicHeurCalculator,
					std::make_shared<ComplexChecker>(createBasicCheckers(basicHeurCalculator)),
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
			Checker::Ptr checker,
			bool log,
			HeurCalculator::Ptr experimentalCalculator)
{
	auto visitedStates = std::make_shared<VisitedStates>();
	NodeFactory::Ptr nodeFactory(new NodeFactory(calculator, experimentalCalculator));
	Expander::Ptr expander = std::make_shared<NormalExpander>(visitedStates,
			calculator, checker, nodeFactory, log);

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
		Checker::Ptr checker = std::make_shared<ComplexChecker>(checkers);
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
			std::make_shared<ComplexChecker>(checkers),
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





