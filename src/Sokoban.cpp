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

#include "BlockListGenerator.hpp"
#include "formatOutput.hpp"
#include "Loader.hpp"
#include "Node.hpp"
#include "PreprocessedResult.hpp"
#include "Solver.hpp"
#include "SolverFactories.hpp"
#include "SubStatusForEach.hpp"

#include "util/ThreadPool.hpp"

#include <boost/filesystem.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/unique_ptr.hpp>
#include <boost/serialization/unordered_set.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/vector.hpp>

#include <iostream>
#include <fstream>
#include <deque>
#include <boost/format.hpp>
#include <functional>

Sokoban::Sokoban(int argc, const char* argv[]):
        options(parseOptions(argc, argv, "sokoban.cfg"))
{
    Status::statusPoolSize(options.statusPoolSize_);

    if (!options.saveProgress_.empty() &&
            boost::filesystem::exists(options.saveProgress_)) {
        load();
    } else if (!options.preprocessLoadFilename_.empty()) {
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

void Sokoban::createExpanderFactory()
{
    assert(table);
    if (!expanderFactory) {
        expanderFactory = std::make_unique<OptionsBasedExpanderFactory>(options,
                *table,
                [this](const BlockListGenerator& blockListGenerator) {
                    savePreprocess(blockListGenerator);
                },
                (options.test_ ? nullptr : &expandedNodes),
                &chokePointFinderTime, &preprocessingIterationTime);
    }
}

int Sokoban::run()
{
    namespace pt = boost::posix_time;

    createExpanderFactory();

    if (progressStatus < ProgressStatus::preprocessing) {
        preprocess();
    }

    auto createExpander = expanderFactory->factory(preprocessedResult);
    Solver solver(
            [this]() { return createPrioQueueFromOptions(options); },
            createExpander,
            [this]() { return createDumperFromOptions(options); });
    std::ofstream heurDump("plusHeur.dump", std::ios::out | std::ios::trunc);
    SolutionChecker solutionChecker(std::cerr, heurDump);
    int returnCode = 0;

    if (options.test_ > 0) {
        std::shared_ptr<const HeurCalculator> calculator =
                expanderFactory->createAdvancedHeurCalcularor(1.0);
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
                ComplexChecker{expanderFactory->createBasicCheckers(calculator)});
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

void Sokoban::preprocess()
{
    if (loader) {
        loader->get() >> preprocessedResult;
        expanderFactory->setHeurCalculatorParameters(
                *preprocessedResult.heurCalculator);
    } else {
        auto blockListGenerator = expanderFactory->createBlockListGenerator();
        preprocessedResult = expanderFactory->preprocess(*blockListGenerator);
    }

    if (!options.preprocessSaveFilename_.empty()) {
        std::ofstream stream{options.preprocessSaveFilename_,
                std::ios::out | std::ios::trunc};
        OutputArchive archive{stream};
        archive << table;
        archive << status;
        archive << preprocessedResult;
    }
}

void Sokoban::saveBasics(OutputArchive& archive)
{
    archive << progressStatus;
    archive << options;
    archive << table;
    archive << status;
}

void Sokoban::savePreprocess(const BlockListGenerator& blockListGenerator)
{
    std::ofstream stream{options.saveProgress_,
            std::ios::out | std::ios::trunc};
    OutputArchive archive{stream};

    progressStatus = ProgressStatus::preprocessing;
    saveBasics(archive);
    archive << blockListGenerator;
}

void Sokoban::load()
{
    Loader loader{options.saveProgress_};
    InputArchive& archive = loader.get();

    archive >> progressStatus;
    archive >> options;
    archive >> table;
    archive >> status;

    switch (progressStatus) {
    case ProgressStatus::preprocessing:
        preprocessedResult = loadPreprocessedResult(archive);
        break;
    default:
        throw std::runtime_error{"Unsupported progress status"};
    }
}

PreprocessedResult Sokoban::loadPreprocessedResult(InputArchive& archive)
{
    createExpanderFactory();
    auto blockListGenerator = expanderFactory->createBlockListGenerator();
    archive >> *blockListGenerator;
    return expanderFactory->preprocess(*blockListGenerator);
}


