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

HeurCalculator::Ptr CreateExpanderFromOptions::createAdvancedHeurCalcularor()
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

std::vector<Checker::Ptr> CreateExpanderFromOptions::createBasicCheckers(const HeurCalculator::Ptr& calculator)
{
	std::vector<Checker::Ptr> checkers;
	if (options_.useMovableChecker())
		checkers.push_back(Checker::Ptr(new MovableChecker(calculator)));
	if (options_.useCorridorChecker())
		checkers.push_back(Checker::Ptr(new CorridorChecker(calculator)));
	return checkers;
}

Expander::Ptr CreateExpanderFromOptions::createExpander(HeurCalculator::Ptr calculator, Checker::Ptr checker, bool log)
{
	VisitedStateHolder::Ptr visitedStates(new VisitedStates());
	NodeFactory::Ptr nodeFactory(new NodeFactory(calculator));
	std::vector<Expander::Ptr> expanders;
	if (options_.useStonePusher()) {
		expanders.push_back(Expander::Ptr(new StonePusher(visitedStates, calculator, nodeFactory)));
	}
	expanders.push_back(Expander::Ptr(new NormalExpander(visitedStates, calculator, checker, nodeFactory, log)));
	return Expander::Ptr(new ComplexExpander(expanders.begin(), expanders.end()));
}

Expander::Ptr CreateExpanderFromOptions::operator()()
{
	HeurCalculator::Ptr calc =
		options_.useAdvancedHeurCalculator() ?
		createAdvancedHeurCalcularor() :
		std::make_shared<BasicHeurCalculator>();
	std::vector<Checker::Ptr> checkers = createBasicCheckers(calc);
	if (options_.blockListStones() > 1) {
		Checker::Ptr checker = std::make_shared<ComplexChecker>(checkers);
		Solver::Ptr solver(new Solver(
				std::bind(&createPrioQueueFromOptions, options_),
				[this, calc, checker]() {
					return createExpander(calc, checker, false);
				})) ;
		BlockListGenerator blockListGenerator(
				solver, calc, checker, options_.blockListStones(), options_.blockListDistance());
		blockListGenerator.init(table_);
		checkers.push_back(blockListGenerator.checker());
	}
	return createExpander(calc, std::make_shared<ComplexChecker>(checkers), log_);
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





