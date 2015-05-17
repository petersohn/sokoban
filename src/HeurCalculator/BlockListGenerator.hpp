#ifndef BLOCKLISTGENERATOR_H
#define BLOCKLISTGENERATOR_H

#include "Checker/ComplexCheckerFwd.hpp"
#include "Checker/ComplexCheckerBase.hpp"
#include "Checker/ComplexCheckerFwd.hpp"

#include "IndexedStatusList.hpp"
#include "Options.hpp"

#include "util/ThreadPool.hpp"
#include "util/TimeMeter.hpp"

#include <boost/asio/io_service.hpp>
#include <boost/date_time.hpp>
#include <boost/thread.hpp>

#include <fstream>
#include <memory>

class HeurCalculator;
class Solver;
class Checker;
struct IncrementInfo;

class BlockListGenerator {
private:

    typedef std::vector<IncrementInfo> IncrementList;

    struct CalculationInfo {
        std::ostringstream dump_;
        std::vector<Status> blockList_;
        IncrementList heurList_;
    };
    typedef std::unique_ptr<CalculationInfo> CalculationInfoPtr;

    std::unique_ptr<const Solver> solver_;
    std::shared_ptr<const HeurCalculator> calculator_;
    std::shared_ptr<const HeurCalculator> incrementalCalculator_;
    ComplexChecker checker_;
    IndexedStatusList blockList_;
    IncrementList heurList_;
    const Table* table_;
    Options options_;
    std::vector<CalculationInfoPtr> calculationInfos_;
    std::ofstream dump_;
    util::ThreadPool threadPool_;
    util::TimerData chokePointFinderTime_;
    util::TimerData iteratingTime_;

    std::deque<std::shared_ptr<Node>> doCalculateBlockList(const Status& status);
    void calculateBlockList(const Status& status);
    void calculateHeurList(const Status& status);
    void dumpStatus(const Status& status, const Point *p, const std::string& title);
    void updateResult();
    Array<bool> calculateChokePoints();

public:
    BlockListGenerator(std::unique_ptr<const Solver> solver, std::shared_ptr<const HeurCalculator> calculator,
            ComplexChecker checker, const Options& options);
    ~BlockListGenerator();
    std::shared_ptr<const Checker> checker();
    std::shared_ptr<const HeurCalculator> vectorHeurCalculator(float heurMultiplier);
    std::shared_ptr<const HeurCalculator> decisionTreeHeurCalculator(std::size_t maxDepth,
            bool useChecker, float heurMultiplier);
    void init(const Table& table);
    util::TimerData chokePointFinderTime() const { return chokePointFinderTime_; }
    util::TimerData iteratingTime() const { return iteratingTime_; }
};

#endif /* BLOCKLISTGENERATOR_H */
