#ifndef SOKOBAN_HPP
#define SOKOBAN_HPP

#include "ArchiveFwd.hpp"
#include "Options.hpp"
#include "PreprocessedResult.hpp"
#include "ProgressStatus.hpp"

#include "util/TimeMeter.hpp"

#include <memory>

class BlockListGenerator;
class Loader;
class OptionsBasedExpanderFactory;
class Status;
class Table;

class Sokoban {
public:
    Sokoban(int argc, const char* argv[]);
    ~Sokoban();

    int run();

private:
    Options options;

    std::unique_ptr<const Table> table;
    std::unique_ptr<Status> status;
    std::unique_ptr<Loader> loader;
    std::unique_ptr<OptionsBasedExpanderFactory> expanderFactory;

    ProgressStatus progressStatus = ProgressStatus::none;
    PreprocessedResult preprocessedResult;

    util::TimeMeter timeMeter;
    std::size_t expandedNodes = 0;
    util::TimerData chokePointFinderTime;
    util::TimerData preprocessingIterationTime;

    void createExpanderFactory();
    void preprocess();

    void saveBasics(OutputArchive& archive);
    void savePreprocess(const BlockListGenerator& blockListGenerator);

    void load();
    PreprocessedResult loadPreprocessedResult(InputArchive& archive);
};

#endif // SOKOBAN_HPP
