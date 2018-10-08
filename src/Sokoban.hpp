#ifndef SOKOBAN_HPP
#define SOKOBAN_HPP

#include "ArchiveFwd.hpp"
#include "Options.hpp"
#include "PreprocessedResult.hpp"
#include "ProgressStatus.hpp"

#include "util/TimeMeter.hpp"

#include <memory>

namespace sokoban {

class Preprocessor;
class Loader;
class OptionsBasedExpanderFactory;
class Status;
class Table;
struct Heuristics;

class Sokoban {
public:
    Sokoban(int argc, const char* argv[]);
    ~Sokoban();

    int run();

private:
    Options options;

    std::unique_ptr<const Table> table;
    std::unique_ptr<Status> initialStatus;
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
    bool sanityCheck(const Heuristics& heuristics);

    void saveBasics(OutputArchive& archive);
    void savePreprocess(const Preprocessor& preprocessor);

    void load();
    PreprocessedResult loadPreprocessedResult(InputArchive& archive);
};

#endif // SOKOBAN_HPP

} // namespace sokoban
