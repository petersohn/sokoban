#include "Sokoban.hpp"

#include "Checker/Checker.hpp"
#include "Checker/ComplexCheckerFwd.hpp"
#include "Checker/SolutionChecker.hpp"

#include "Dumper/DumperFunctions.hpp"

#include "HeurCalculator/HeurCalculator.hpp"

#include "Status/State.hpp"
#include "Status/Status.hpp"
#include "Status/StatusCreator.hpp"
#include "Status/Table.hpp"

#include "formatOutput.hpp"
#include "Loader.hpp"
#include "Node.hpp"
#include "Solver.hpp"
#include "SolverFactories.hpp"
#include "SubStatusForEach.hpp"

#include "util/ThreadPool.hpp"

#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/unique_ptr.hpp>

#include <iostream>
#include <fstream>
#include <deque>
#include <boost/format.hpp>
#include <functional>

Sokoban::Sokoban(int argc, const char* argv[]):
        options(parseOptions(argc, argv, "sokoban.cfg"))
{
    Status::statusPoolSize(options.statusPoolSize_);

    if (!options.preprocessLoadFilename_.empty()) {
        loader = std::make_unique<Loader>(options.preprocessLoadFilename_);
        loader->get() >> table;
        loader->get() >> status;
    } else {
        auto data(loadStatusFromFile(options.filename_.c_str()));
        table = std::move(data.first);
        status = std::make_unique<Status>(std::move(data.second));
    }
    dumpStatus(std::cerr, *status);

}

Sokoban::~Sokoban() = default;

namespace {

void solveTestProblem(SolutionChecker& solutionChecker, Solver& solver, Status status)
{
    std::deque<std::shared_ptr<Node>> solution = solver.solve(std::move(status));
    if (!solution.empty()) {
        solutionChecker.checkResult(status, solution);
    }
}

}

int Sokoban::run()
{
    namespace pt = boost::posix_time;
    OptionsBasedExpanderFactory expanderFactory(options, status->table(),
            (options.test_ ? nullptr : &expandedNodes),
            &chokePointFinderTime, &preprocessingIterationTime);

    PreprocessedResult preprocessedResult;

    if (loader) {
        loader->get() >> preprocessedResult;
        expanderFactory.setHeurCalculatorParameters(
                *preprocessedResult.heurCalculator);
    } else {
        preprocessedResult = expanderFactory.preprocess();
    }

    if (!options.preprocessSaveFilename_.empty()) {
        std::ofstream stream{options.preprocessSaveFilename_,
                std::ios::out | std::ios::trunc};
        OutputArchive archive{stream};
        archive << table;
        archive << status;
        archive << preprocessedResult;
    }

    auto createExpander = expanderFactory.factory(preprocessedResult);
    Solver solver(
            [this]() { return createPrioQueueFromOptions(options); },
            createExpander,
            [this]() { return createDumperFromOptions(options); });
    std::ofstream heurDump("plusHeur.dump", std::ios::out | std::ios::trunc);
    SolutionChecker solutionChecker(std::cerr, heurDump);
    int returnCode = 0;

    if (options.test_ > 0) {
        std::shared_ptr<const HeurCalculator> calculator =
                expanderFactory.createAdvancedHeurCalcularor(1.0);
        util::ThreadPool threadPool;
        util::ThreadPoolRunner runner(threadPool);
        threadPool.setNumThreads(options.numThreads_);
        SubStatusForEach it(status->table(),
                std::bind(solveTestProblem, std::ref(solutionChecker),
                        std::ref(solver), std::placeholders::_1),
                SubStatusForEach::MinDistance{0}, SubStatusForEach::MaxDistance{0},
                SubStatusForEach::ChokePointDistantNum{0}, {},
                SubStatusForEach::WorkQueueLength{options.workQueueLength_},
                SubStatusForEach::ReverseSearchMaxDepth{0},
                threadPool.getIoService());
        it.start(options.test_, calculator,
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
            std::ofstream dump(options.solutionDumpFilename_,
                    std::ios::out | std::ios::trunc);
            dump << formatOutput("%solution:dump%", solutionData);
        }

        std::cout << formatOutput(options.outputFormat_, solutionData);
    }
    return returnCode;
}



