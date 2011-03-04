#include "BasicSolver.h"
#include "PrioNodeQueue.h"
#include "ComplexExpander.h"
#include "NormalExpander.h"
#include "StonePusher.h"
#include "ComplexChecker.h"
#include "MovableChecker.h"
#include "CorridorChecker.h"
#include "BasicHeurCalculator.h"
#include "AdvancedHeurCalculator.h"
#include <vector>

Expander::Ptr BasicSolver::createExpander()
{
	HeurCalculator::Ptr calc(new AdvancedHeurCalculator(
			HeurCalculator::Ptr(new BasicHeurCalculator)));
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

NodeQueue::Ptr BasicSolver::createQueue()
{
	return NodeQueue::Ptr(new PrioNodeQueue());
}

