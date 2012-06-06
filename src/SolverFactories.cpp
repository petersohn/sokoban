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
#include "BlockListChecker.h"
#include "BasicHeurCalculator.h"
#include "AdvancedHeurCalculator.h"
#include "VisitedStates.h"
#include "CompareQueue.h"
#include "TextDumper.h"
#include "XDumper.h"
#include "NodeFactory.h"
#include <vector>
#include <functional>

NodeQueue::Ptr createPrioQueue()
{
	std::vector<CompareQueue<Node::Ptr>::FuncType> funcs;
	funcs.push_back(CompareByMethodPtr<Node::Ptr>(&Node::costFgv, false));
	funcs.push_back(CompareByMethodPtr<Node::Ptr>(&Node::depth, true));
	return NodeQueue::Ptr(new PrioNodeQueue<CompareQueue<Node::Ptr> >(CompareQueue<Node::Ptr>(
			funcs.begin(), funcs.end())));
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

static HeurCalculator::Ptr createAdvancedHeurCalcularor()
{
	HeurCalculator::Ptr bhc(new BasicHeurCalculator);
	Solver::Ptr s(new Solver(createPrioQueue,
					std::bind(createExpanderWithCalculator, bhc, false)));
	return HeurCalculator::Ptr(new AdvancedHeurCalculator(s));
}

Expander::Ptr createExpander(bool log) {
	return createExpanderWithCalculator(createAdvancedHeurCalcularor(), log);
}

Expander::Ptr createExpanderWithCalculator(HeurCalculator::Ptr calc, bool log)
{
	std::vector<Checker::Ptr> chs;
	chs.push_back(Checker::Ptr(new MovableChecker(calc)));
	chs.push_back(Checker::Ptr(new CorridorChecker(calc)));
	Checker::Ptr ch(new ComplexChecker(chs.begin(), chs.end()));
	VisitedStateHolder::Ptr vs(new VisitedStates());
	NodeFactory::Ptr nf(new NodeFactory(calc));
	std::vector<Expander::Ptr> exs;
//	exs.push_back(Expander::Ptr(new StonePusher(vs, calc)));
	exs.push_back(Expander::Ptr(new NormalExpander(vs, calc, ch, nf, log)));
	return Expander::Ptr(new ComplexExpander(exs.begin(), exs.end()));
}

static Expander::Ptr createExpanderFromOptions0(const Options &opts, FixedTable::Ptr table, bool log, bool blocklist)
{
	HeurCalculator::Ptr calc =
			opts.useAdvancedHeurCalculator() ?
			createAdvancedHeurCalcularor() :
			std::make_shared<BasicHeurCalculator>();
	std::vector<Checker::Ptr> chs;
	if (opts.useMovableChecker())
		chs.push_back(Checker::Ptr(new MovableChecker(calc)));
	if (opts.useCorridorChecker())
		chs.push_back(Checker::Ptr(new CorridorChecker(calc)));
	if (blocklist && opts.blockListStones() > 1) {
		Solver::Ptr solver(new Solver(
				std::bind(&createPrioQueueFromOptions, opts),
				std::bind(createExpanderFromOptions0, opts, table, false, false)));
		Checker::Ptr ch(new ComplexChecker(chs.begin(), chs.end()));
		std::shared_ptr<BlockListChecker> blocklistChecker(
				new BlockListChecker(solver, calc, ch, opts.blockListStones(), opts.blockListDistance(),
				opts.progressInterval()));
		blocklistChecker->init(table);
		chs.push_back(blocklistChecker);
	}
	Checker::Ptr ch(new ComplexChecker(chs.begin(), chs.end()));
	VisitedStateHolder::Ptr vs(new VisitedStates());
	NodeFactory::Ptr nf(new NodeFactory(calc));
	std::vector<Expander::Ptr> exs;
	if (opts.useStonePusher())
		exs.push_back(Expander::Ptr(new StonePusher(vs, calc, nf)));
	exs.push_back(Expander::Ptr(new NormalExpander(vs, calc, ch, nf, log)));
	return Expander::Ptr(new ComplexExpander(exs.begin(), exs.end()));
}

Expander::Ptr createExpanderFromOptions(const Options &opts, FixedTable::Ptr table, bool log)
{
	return createExpanderFromOptions0(opts, table, log, true);
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





