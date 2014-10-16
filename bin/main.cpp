#include "Common.hpp"
#include "Status/Status.hpp"
#include "Options.hpp"
#include "Node.hpp"
#include "Solver.hpp"
#include "SolverFactories.hpp"
#include "Dumper/DumperFunctions.hpp"
#include "Status/State.hpp"
#include "SolutionChecker.hpp"
#include "TableIterator.hpp"
#include "ComplexChecker.hpp"
#include "TimeMeter.hpp"
#include "Status/StatusCreator.hpp"
#include <iostream>
#include <fstream>
#include <deque>
#include <boost/format.hpp>
#include <functional>

using namespace std;
namespace pt = boost::posix_time;


void solveTestProblem(SolutionChecker& solutionChecker, Solver& solver, Status status)
{
	std::deque<Node::Ptr> solution = solver.solve(std::move(status));
	if (!solution.empty()) {
		solutionChecker.checkResult(status, solution);
	}
}

int main(int argc, char** argv) {
	Options opts = parseOptions(argc, argv, "sokoban.cfg");

	Status::statusPoolSize(opts.statusPoolSize_);

	auto data(loadStatusFromFile(opts.filename_.c_str()));
	Status& status = data.second;
	dumpStatus(std::cerr, status);

	TimeMeter timeMeter;
	OptionsBasedExpanderFactory expanderFactory(opts, status.table(), !opts.test_);
	auto createExpander = expanderFactory.factory();
	Solver s(std::bind(createPrioQueueFromOptions, opts),
			createExpander,
			std::bind(createDumperFromOptions, opts),
			opts.parallelOuterExpand_ ? opts.numThreads_ : 0);
	std::ofstream heurDump("plusHeur.dump", std::ios::out | std::ios::trunc);
	SolutionChecker solutionChecker(std::cerr, heurDump);
	int returnCode = 0;
	if (opts.test_ > 0) {
		HeurCalculator::Ptr calculator = expanderFactory.createAdvancedHeurCalcularor();
		util::ThreadPool threadPool;
		util::ThreadPoolRunner runner(threadPool);
		threadPool.setNumThreads(opts.numThreads_);
		TableIterator it(status.table(),
				std::bind(solveTestProblem, std::ref(solutionChecker), std::ref(s), std::placeholders::_1),
				0, opts.workQueueLength_, threadPool);
		it.iterate(opts.test_, calculator,
				std::make_shared<ComplexChecker>(expanderFactory.createBasicCheckers(calculator)));
	} else {
		std::deque<Node::Ptr> solution = s.solve(status);
		cerr << "Length of solution: " << solution.size() << endl;
		cerr << "Processor Time:" << timeMeter.processorTime() << endl;
		cerr << "Real Time:" << timeMeter.realTime() << endl;
		if (solution.empty())
			cerr << "No solution." << endl;
		else
		{
			if (solutionChecker.checkResult(status, solution)) {
				cerr << "Solution OK." << endl;
			} else {
				cerr << "Solution bad." << endl;
				returnCode = 1;
			}
			std::ofstream dump("solution.dump", std::ios::out | std::ios::trunc);
			for (std::deque<Node::Ptr>::iterator it = solution.begin();
					it != solution.end(); ++it)
			{
				dumpNode(dump, status.table(), **it);
				Point from = (*it)->from();
				Point p(from - (*it)->d());
				std::string dir =
						! opts.oldStyleOutput_ ? direction((*it)->d()) :
							(boost::format("(%2d, %2d)") %
							p.x % p.y).str();
				cout << boost::format("(%2d, %2d) --> %s") %
						from.x % from.y % dir << endl;
			}
		}
	}
	cerr << "Status copied " << Status::copyCount << " times.\n" <<
			"Status moved " << Status::moveCount << " times.\n" <<
			"calculateReachable called " << Status::calculateReachableCount << " times.\n" <<
			"Status pool size = " << Status::currentStatusPoolSize() << endl;
	return returnCode;
}
