#include "Status/Status.hpp"
#include "Options.hpp"
#include "Node.hpp"
#include "Solver.hpp"
#include "SolverFactories.hpp"
#include "Dumper/DumperFunctions.hpp"
#include "Status/State.hpp"
#include "SolutionChecker.hpp"
#include "SubStatusForEach.hpp"
#include "ComplexCheckerFwd.hpp"
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
	const auto& table = data.first;
	Status& status = data.second;
	dumpStatus(std::cerr, status);

	util::TimeMeter timeMeter;
	std::size_t expandedNodes = 0;
	util::TimerData chokePointFinderTime;
	util::TimerData preprocessingIterationTime;
	OptionsBasedExpanderFactory expanderFactory(opts, status.table(),
			(opts.test_ ? nullptr : &expandedNodes),
			&chokePointFinderTime, &preprocessingIterationTime);
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
		SolutionData solutionData{*table, solution,
				SolutionQuality::none, ExpandedNodes{expandedNodes},
				TotalTime{timeMeter.data()},
				ChokePointFindingTime{chokePointFinderTime},
				PreprocessingIterationTime{preprocessingIterationTime}};
		if (!solution.empty())
		{
			if (solutionChecker.checkResult(status, solution)) {
				solutionData.solutionQuality = SolutionQuality::good;
			} else {
				solutionData.solutionQuality = SolutionQuality::bad;
				returnCode = 1;
			}
			std::ofstream dump(opts.solutionDumpFilename_,
					std::ios::out | std::ios::trunc);
			dump << formatOutput("%solution:dump%", solutionData);
		}

		std::cout << formatOutput(opts.outputFormat_, solutionData);
	}
	return returnCode;
}
