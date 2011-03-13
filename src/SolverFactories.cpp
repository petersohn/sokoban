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
#include "BasicHeurCalculator.h"
#include "AdvancedHeurCalculator.h"
#include "VisitedStates.h"
#include "CompareQueue.h"
#include "TextDumper.h"
#include "XDumper.h"
#include <vector>
#include <boost/bind.hpp>

NodeQueue::Ptr createPrioQueue()
{
	std::vector<CompareQueue<Node::Ptr>::FuncType> funcs;
	funcs.push_back(CompareByMethodPtr<Node::Ptr>(&Node::costFgv, false));
	funcs.push_back(CompareByMethodPtr<Node::Ptr>(&Node::depth, true));
	return NodeQueue::Ptr(new PrioNodeQueue<CompareQueue<Node::Ptr> >(CompareQueue<Node::Ptr>(
			funcs.begin(), funcs.end())));
}

static HeurCalculator::Ptr createAdvancedHeurCalcularor()
{
	HeurCalculator::Ptr bhc(new BasicHeurCalculator);
	Solver::Ptr s(new Solver(createPrioQueue,
					boost::bind(createExpanderWithCalculator, bhc, false)));
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
	std::vector<Expander::Ptr> exs;
	exs.push_back(Expander::Ptr(new StonePusher(vs, calc)));
	exs.push_back(Expander::Ptr(new NormalExpander(vs, calc, ch, log)));
	return Expander::Ptr(new ComplexExpander(exs.begin(), exs.end()));
}

Expander::Ptr createExpanderFromOptions(const Options &opts, bool log)
{
	HeurCalculator::Ptr calc = createAdvancedHeurCalcularor();
	std::vector<Checker::Ptr> chs;
	if (opts.useMovableChecker())
		chs.push_back(Checker::Ptr(new MovableChecker(calc)));
	if (opts.useCorridorChecker())
		chs.push_back(Checker::Ptr(new CorridorChecker(calc)));
	Checker::Ptr ch(new ComplexChecker(chs.begin(), chs.end()));
	VisitedStateHolder::Ptr vs(new VisitedStates());
	std::vector<Expander::Ptr> exs;
	if (opts.useStonePusher())
		exs.push_back(Expander::Ptr(new StonePusher(vs, calc)));
	exs.push_back(Expander::Ptr(new NormalExpander(vs, calc, ch, log)));
	return Expander::Ptr(new ComplexExpander(exs.begin(), exs.end()));
}

Dumper::Ptr createDumperFromOptions(const Options & opts)
{
	switch (opts.dumpStyle()) {
	case Options::dsText:
		return Dumper::Ptr(new TextDumper("dump.dump"));
	case Options::dsXML:
		return Dumper::Ptr(new TextDumper("dump.xml"));
	}
	return Dumper::Ptr();
}





