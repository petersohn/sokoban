#include "SolverFactories.h"
#include "Solver.h"
#include "PrioNodeQueue.h"
#include "ComplexExpander.h"
#include "NormalExpander.h"
#include "StonePusher.h"
#include "ComplexStrategy.h"
#include "ComplexChecker.h"
#include "MovableChecker.h"
#include "CorridorChecker.h"
#include "BlockListGenerator.h"
#include "BasicHeurCalculator.h"
#include "AdvancedHeurCalculator.h"
#include "VisitedStates.h"
#include "CompareQueue.h"
#include "TextDumper.h"
#include "XDumper.h"
#include "NodeFactory.h"
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
	// always use costFgv as first choice
	funcs.push_back(CompareByMethodPtr<Node::Ptr>(&Node::costFgv, false));
	for (Options::CompareList::const_iterator it = opts.compare().begin();
			it != opts.compare().end(); ++it) {
		CompareByMethodPtr<Node::Ptr>::Fun fun;
		switch (it->type) {
		case Options::ctHeur: fun = &Node::heur; break;
		case Options::ctDepth: fun = &Node::depth; break;
		case Options::ctTime: fun = &Node::time; break;
		}
		if (fun)
			funcs.push_back(CompareByMethodPtr<Node::Ptr>(fun, it->reverse));
	}
	return NodeQueue::Ptr(new PrioNodeQueue<CompareQueue<Node::Ptr> >(CompareQueue<Node::Ptr>(
			funcs.begin(), funcs.end())));
}

HeurCalculator::Ptr OptionsBasedExpanderFactory::createAdvancedHeurCalcularor()
{
	HeurCalculator::Ptr bhc(new BasicHeurCalculator);
	Solver::Ptr s(new Solver(createPrioQueue,
		[this, bhc]() {
			return createExpander(
					bhc,
					std::make_shared<ComplexChecker>(createBasicCheckers(bhc)),
					false);
		}));
	return HeurCalculator::Ptr(new AdvancedHeurCalculator(s));
}

std::vector<Checker::Ptr> OptionsBasedExpanderFactory::createBasicCheckers(const HeurCalculator::Ptr& calculator)
{
	std::vector<Checker::Ptr> checkers;
	if (options_.useMovableChecker())
		checkers.push_back(Checker::Ptr(new MovableChecker(calculator)));
	if (options_.useCorridorChecker())
		checkers.push_back(Checker::Ptr(new CorridorChecker(calculator)));
	return checkers;
}

Expander::Ptr OptionsBasedExpanderFactory::createExpander(
			HeurCalculator::Ptr calculator,
			Checker::Ptr checker,
			bool log,
			HeurCalculator::Ptr experimentalCalculator)
{
	VisitedStateHolder::Ptr visitedStates(new VisitedStates());
	NodeFactory::Ptr nodeFactory(new NodeFactory(calculator, experimentalCalculator));
	std::vector<Expander::Ptr> expanders;
	if (options_.useStonePusher()) {
		expanders.push_back(Expander::Ptr(new StonePusher(visitedStates, calculator, nodeFactory)));
	}
	expanders.push_back(Expander::Ptr(new NormalExpander(visitedStates, calculator, checker, nodeFactory, log)));
	return Expander::Ptr(new ComplexExpander(expanders.begin(), expanders.end()));
}

ExpanderFactory OptionsBasedExpanderFactory::factory()
{
	HeurCalculator::Ptr calculator =
		options_.useAdvancedHeurCalculator() ?
		createAdvancedHeurCalcularor() :
		std::make_shared<BasicHeurCalculator>();
	HeurCalculator::Ptr experimentalCalculator;
	std::vector<Checker::Ptr> checkers = createBasicCheckers(calculator);
	if (options_.blockListStones() > 1) {
		Checker::Ptr checker = std::make_shared<ComplexChecker>(checkers);
		Solver::Ptr solver(new Solver(
				std::bind(&createPrioQueueFromOptions, options_),
				[this, calculator, checker]() {
					return createExpander(calculator, checker, false);
				})) ;
		BlockListGenerator blockListGenerator(
				solver, calculator, checker, options_.blockListStones(),
				options_.blockListDistance(), options_.maxHeurListSize());
		blockListGenerator.init(table_);
		checkers.push_back(blockListGenerator.checker());
//		experimentalCalculator = blockListGenerator.heurCalculator();
		if (options_.useBlocklistHeurCalculator()) {
			calculator = blockListGenerator.heurCalculator();
		}
	}
	return std::bind(&OptionsBasedExpanderFactory::createExpander, this,
			calculator,
			std::make_shared<ComplexChecker>(checkers),
			log_,
			experimentalCalculator);
}

Dumper::Ptr createDumperFromOptions(const Options & opts)
{
	switch (opts.dumpStyle()) {
	case Options::dsText:
		return Dumper::Ptr(new TextDumper("dump.dump"));
	case Options::dsXML:
		return Dumper::Ptr(new XDumper("dump.xml"));
	default:
		return Dumper::Ptr();
	}
}





