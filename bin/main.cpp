#include "Status/Status.hpp"
#include "Options.hpp"
#include "Node.hpp"
#include "Solver.hpp"
#include "SolverFactories.hpp"
#include "Dumper/DumperFunctions.hpp"
#include "Status/State.hpp"
#include "SolutionChecker.hpp"
#include "SubStatusForEach.hpp"
#include "ComplexChecker.hpp"
#include "util/TimeMeter.hpp"
#include "Status/StatusCreator.hpp"
#include "util/ThreadPool.hpp"
#include "formatOutput.hpp"
#include <iostream>
#include <fstream>
#include <deque>
#include <boost/format.hpp>
#include <functional>

using namespace std;
namespace pt = boost::posix_time;


void solveTestProblem(SolutionChecker& solutionChecker, Solver& solver, Status status)
{
	std::deque<std::shared_ptr<Node>> solution = solver.solve(std::move(status));
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

	util::TimeMeter timeMeter;
	OptionsBasedExpanderFactory expanderFactory(opts, status.table(), !opts.test_);
	auto createExpander = expanderFactory.factory();
	Solver s(std::bind(createPrioQueueFromOptions, opts),
			createExpander, std::bind(createDumperFromOptions, opts));
	std::ofstream heurDump("plusHeur.dump", std::ios::out | std::ios::trunc);
	SolutionChecker solutionChecker(std::cerr, heurDump);
	int returnCode = 0;
	if (opts.test_ > 0) {
		std::shared_ptr<const HeurCalculator> calculator = expanderFactory.createAdvancedHeurCalcularor(
				1.0);
		util::ThreadPool threadPool;
		util::ThreadPoolRunner runner(threadPool);
		threadPool.setNumThreads(opts.numThreads_);
		SubStatusForEach it(status.table(),
				std::bind(solveTestProblem, std::ref(solutionChecker),
					std::ref(s), std::placeholders::_1),
				SubStatusForEach::MinDistance{0}, SubStatusForEach::MaxDistance{0},
				SubStatusForEach::ChokePointDistantNum{0}, {},
				SubStatusForEach::WorkQueueLength{opts.workQueueLength_},
				SubStatusForEach::ReverseSearchMaxDepth{0},
				threadPool.getIoService());
		it.start(opts.test_, calculator,
				ComplexChecker{expanderFactory.createBasicCheckers(calculator)});
		it.wait(true);
	} else {
		std::deque<std::shared_ptr<Node>> solution = s.solve(status);
		cerr << "Length of solution: " << solution.size() << endl;
		cerr << "Processor Time:" << timeMeter.processorTime() << endl;
		cerr << "Real Time:" << timeMeter.realTime() << endl;
		SolutionQuality solutionQuality = SolutionQuality::none;
		if (solution.empty())
			cerr << "No solution." << endl;
		else
		{
			if (solutionChecker.checkResult(status, solution)) {
				cerr << "Solution OK." << endl;
				solutionQuality = SolutionQuality::good;
			} else {
				cerr << "Solution bad." << endl;
				solutionQuality = SolutionQuality::bad;
				returnCode = 1;
			}
			std::ofstream dump(opts.solutionDumpFilename_,
					std::ios::out | std::ios::trunc);
			for (std::deque<std::shared_ptr<Node>>::iterator it = solution.begin();
					it != solution.end(); ++it)
			{
				dumpNode(dump, status.table(), **it);
			}
		}

		std::cout << formatOutput(opts.outputFormat_, solution,
				solutionQuality);
	}
#ifndef NO_UNSAFE_DIAGNOSTICS
	cerr << "Status copied " << Status::copyCount << " times.\n" <<
			"Status moved " << Status::moveCount << " times.\n" <<
			"calculateReachable called " << Status::calculateReachableCount << " times.\n" <<
			"Status pool size = " << Status::currentStatusPoolSize() << endl;
#endif
	return returnCode;
}
