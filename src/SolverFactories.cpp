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
#include <vector>
#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>

NodeQueue::Ptr createPrioQueue()
{
//	using namespace boost::lambda;
	typedef ComplexStrategy<std::pair<const Node&, const Node&>, int, 0> StrategyType;
	std::vector<StrategyType::FuncType> funcs;
	funcs.push_back(boost::lambda::_1.costFgv() - boost::lambda::_2.costFgv());
	funcs.push_back(_2.depth() - _1.depth());
	StrategyType comp(funcs.begin(), funcs.end(), _2, _1 != 0);
	return NodeQueue::Ptr(new PrioNodeQueue(comp(std::make_pair(_1, _2)) > 0));
}

Expander::Ptr createExpander() {
	return createExpanderWithCalculator(HeurCalculator::Ptr(
			new AdvancedHeurCalculator(Solver::Ptr(new Solver(
					createPrioQueue,
					boost::bind(createExpanderWithCalculator,
							HeurCalculator::Ptr(new BasicHeirCalculator)))))));
}

Expander::Ptr createExpanderWithCalculator(HeurCalculator::Ptr calc)
{
	std::vector<Checker::Ptr> chs;
	chs.push_back(Checker::Ptr(new MovableChecker(calc)));
	chs.push_back(Checker::Ptr(new CorridorChecker(calc)));
	Checker::Ptr ch(new ComplexChecker(chs.begin(), chs.end()));
	VisitedStateHolder::Ptr vs(new VisitedStates());
	std::vector<Expander::Ptr> exs;
//	exs.push_back(Expander::Ptr(new StonePusher(vs, calc)));
	exs.push_back(Expander::Ptr(new NormalExpander(vs, calc, ch)));
	return Expander::Ptr(new ComplexExpander(exs.begin(), exs.end()));
}

