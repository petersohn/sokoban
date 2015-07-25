#ifndef PREPROCESSOR_HPP
#define PREPROCESSOR_HPP

#include "Checker/ComplexCheckerFwd.hpp"
#include "Checker/ComplexCheckerBase.hpp"
#include "Checker/ComplexCheckerFwd.hpp"
#include "Checker/VisitedStateInfo.hpp"

#include "HeurCalculator/HeurInfo.hpp"

#include "IndexedStatusList.hpp"
#include "Options.hpp"

#include "util/ThreadPool.hpp"
#include "util/TimeMeter.hpp"

#include <boost/date_time.hpp>
#include <boost/thread.hpp>

#include <fstream>
#include <memory>

class HeurCalculator;
class Solver;
class Checker;
class SubStatusForEach;

struct IncrementInfo {
    HeurInfo heurInfo_;
    float difference_;
    IncrementInfo(HeurInfo heurInfo, float difference):
        heurInfo_(std::move(heurInfo)),
        difference_(difference)
    {}
    IncrementInfo(const IncrementInfo& ) = default;
    IncrementInfo(IncrementInfo&& ) = default;
    IncrementInfo& operator=(const IncrementInfo& ) = default;
    IncrementInfo& operator=(IncrementInfo&& ) = default;

    static const HeurInfo& getHeurInfo(const IncrementInfo& incrementInfo) {
        return incrementInfo.heurInfo_;
    }
};

template <typename Ar>
void serialize(Ar& ar, IncrementInfo& incrementInfo,
        const unsigned int /*version*/) {
    ar & incrementInfo.heurInfo_;
    ar & incrementInfo.difference_;
}

template <typename Archive>
void save_construct_data(Archive& ar, const IncrementInfo* incrementInfo,
        const unsigned int /*version*/)
{
    const Table* table = &incrementInfo->heurInfo_.first.table();
    ar << table;
}

template <typename Archive>
void load_construct_data(Archive& ar, IncrementInfo* incrementInfo,
        const unsigned int /*version*/)
{
    Table* table;
    ar >> table;
    Status status{*table};
    ::new(incrementInfo)IncrementInfo{{status, 0.0}, 0.0};
}

class Preprocessor {
public:
    using Saver = std::function<void(const Preprocessor&)>;
private:
    using IncrementList = std::vector<IncrementInfo>;

    struct CalculationInfo {
        std::ostringstream dump_;
        std::vector<Status> blockList_;
        std::unordered_set<VisitedStateInfo> calculatedStatuses_;
        IncrementList heurList_;
        std::size_t callNum_ = 0;
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
    Saver saver_;

    std::unique_ptr<SubStatusForEach> subStatusForEach_;
    std::vector<CalculationInfoPtr> calculationInfos_;
    std::unordered_set<VisitedStateInfo> calculatedStatuses_;
    std::size_t currentStoneNum_;
    std::ofstream dump_;
    util::ThreadPool threadPool_;
    util::TimerData chokePointFinderTime_;
    util::TimerData iteratingTime_;
    util::TimerData savingTime_;

    std::deque<std::shared_ptr<Node>> calculateBlockList(const Status& status);
    void calculateHeurList(const Status& status);
    void dumpStatus(const Status& status, const Point *p,
            const std::string& title);
    void updateResult();
    std::size_t aggregateThreadResults();
    Array<bool> calculateChokePoints();
    void save();

public:
    Preprocessor(std::unique_ptr<const Solver> solver,
            std::shared_ptr<const HeurCalculator> calculator,
            ComplexChecker checker, const Options& options, Saver saver);
    ~Preprocessor();
    std::shared_ptr<Checker> checker();
    std::shared_ptr<HeurCalculator> vectorHeurCalculator(float heurMultiplier);
    std::shared_ptr<HeurCalculator> decisionTreeHeurCalculator(
            std::size_t maxDepth, bool useChecker, float heurMultiplier);

    void init(const Table& table);
    void run();
    util::TimerData chokePointFinderTime() const
    {
        return chokePointFinderTime_;
    }
    util::TimerData iteratingTime() const { return iteratingTime_; }

    template <typename Ar>
    void serialize(Ar& ar, const unsigned int /*version*/) {
        ar & currentStoneNum_;
        ar & blockList_;
        ar & heurList_;
        ar & calculatedStatuses_;
    }
};

#endif // PREPROCESSOR_HPP
