#include "Checker/Checker.hpp"
#include "Checker/ComplexCheckerFwd.hpp"
#include "Checker/SolutionChecker.hpp"

#include "Dumper/DumperFunctions.hpp"

#include "HeurCalculator/HeurCalculator.hpp"

#include "Status/State.hpp"
#include "Status/Status.hpp"
#include "Status/StatusCreator.hpp"

#include "formatOutput.hpp"
#include "Loader.hpp"
#include "Node.hpp"
#include "Options.hpp"
#include "Solver.hpp"
#include "SolverFactories.hpp"
#include "SubStatusForEach.hpp"

#include "util/TimeMeter.hpp"
#include "util/ThreadPool.hpp"

#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/unique_ptr.hpp>

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

    std::unique_ptr<const Table> table;
    std::unique_ptr<Status> status;
    std::unique_ptr<Loader> loader;

    if (!opts.preprocessLoadFilename_.empty()) {
        loader = std::make_unique<Loader>(opts.preprocessLoadFilename_);
        loader->get() >> table;
        loader->get() >> status;
    } else {
        auto data(loadStatusFromFile(opts.filename_.c_str()));
        table = std::move(data.first);
        status = std::make_unique<Status>(std::move(data.second));
    }
    dumpStatus(std::cerr, *status);

    util::TimeMeter timeMeter;
    std::size_t expandedNodes = 0;
    util::TimerData chokePointFinderTime;
    util::TimerData preprocessingIterationTime;
    OptionsBasedExpanderFactory expanderFactory(opts, status->table(),
            (opts.test_ ? nullptr : &expandedNodes),
            &chokePointFinderTime, &preprocessingIterationTime);

    PreprocessedResult preprocessedResult;

    if (loader) {
        loader->get() >> preprocessedResult;
        expanderFactory.setHeurCalculatorParameters(
                *preprocessedResult.heurCalculator);
    } else {
        preprocessedResult = expanderFactory.preprocess();
    }

    if (!opts.preprocessSaveFilename_.empty()) {
        std::ofstream stream{opts.preprocessSaveFilename_,
                std::ios::out | std::ios::trunc};
        OutputArchive archive{stream};
        archive << table;
        archive << status;
        archive << preprocessedResult;
    }

    auto createExpander = expanderFactory.factory(preprocessedResult);
    Solver solver(
            [&opts]() { return createPrioQueueFromOptions(opts); },
            createExpander,
            [&opts]() { return createDumperFromOptions(opts); });
    std::ofstream heurDump("plusHeur.dump", std::ios::out | std::ios::trunc);
    SolutionChecker solutionChecker(std::cerr, heurDump);
    int returnCode = 0;

    if (opts.test_ > 0) {
        std::shared_ptr<const HeurCalculator> calculator =
                expanderFactory.createAdvancedHeurCalcularor(1.0);
        util::ThreadPool threadPool;
        util::ThreadPoolRunner runner(threadPool);
        threadPool.setNumThreads(opts.numThreads_);
        SubStatusForEach it(status->table(),
                std::bind(solveTestProblem, std::ref(solutionChecker),
                    std::ref(solver), std::placeholders::_1),
                SubStatusForEach::MinDistance{0}, SubStatusForEach::MaxDistance{0},
                SubStatusForEach::ChokePointDistantNum{0}, {},
                SubStatusForEach::WorkQueueLength{opts.workQueueLength_},
                SubStatusForEach::ReverseSearchMaxDepth{0},
                threadPool.getIoService());
        it.start(opts.test_, calculator,
                ComplexChecker{expanderFactory.createBasicCheckers(calculator)});
        it.wait(true);
    } else {
        std::deque<std::shared_ptr<Node>> solution = solver.solve(*status);
        SolutionData solutionData{*table, solution,
                SolutionQuality::none, ExpandedNodes{expandedNodes},
                TotalTime{timeMeter.data()},
                ChokePointFindingTime{chokePointFinderTime},
                PreprocessingIterationTime{preprocessingIterationTime}};
        if (!solution.empty())
        {
            if (solutionChecker.checkResult(*status, solution)) {
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
