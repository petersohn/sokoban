#ifndef SRC_SUBSTATUSFOREACH_HPP
#define SRC_SUBSTATUSFOREACH_HPP

#include "Checker/ComplexCheckerBase.hpp"
#include "Checker/ComplexCheckerFwd.hpp"
#include "Status/PointRange.hpp"
#include "Array.hpp"
#include "MultiThreadWorker.hpp"

#include "util/ExplicitType.hpp"
#include "util/TimeMeter.hpp"

#include <boost/optional.hpp>

#include <functional>

namespace boost { namespace asio { class io_service; }}

namespace sokoban {

class State;
class Status;
class HeurCalculator;
class Table;

class SubStatusForEach {
public:
    using Action = std::function<void(const Status&, std::size_t)>;

private:
    MultiThreadWorker worker;

    const Table* table_;
    std::shared_ptr<const HeurCalculator> heurCalculator_;
    ComplexChecker checker_;
    Action action_;
    std::size_t minDistance_;
    std::size_t maxDistance_;
    std::size_t chokePointDistantNum_;
    std::size_t reverseSearchMaxDepth_;
    PointRange range_;
    Array<bool> excludeList_;
    Array<bool> chokePoints_;
    util::TimeMeter timeMeter_;
    std::size_t startingIndex_;
    std::size_t index_;

    void initIter(PointRange::iterator it, std::size_t stones,
            const State& state);
    bool checkDistances(const State& state, Point p);
    bool processState(const State& state, std::size_t stones);

public:

    using MinDistance = util::ExplicitType<struct tag_MinDistance, std::size_t>;
    using MaxDistance = util::ExplicitType<struct tag_MaxDistance, std::size_t>;
    using ChokePointDistantNum =
        util::ExplicitType<struct tag_ChokePointDistantNum, std::size_t>;
    using WorkQueueLength =
        util::ExplicitType<struct tag_WorkQueueLength, std::size_t>;
    using ReverseSearchMaxDepth =
        util::ExplicitType<struct tag_ReverseSearchMaxDepth, std::size_t>;

    SubStatusForEach(
            const Table& table,
            const Action& action,
            MinDistance minDistance,
            MaxDistance maxDistance,
            ChokePointDistantNum chokePointDistantNum,
            Array<bool> chokePoints,
            WorkQueueLength workQueueLength,
            ReverseSearchMaxDepth reverseSearchMaxDepth,
            boost::asio::io_service& ioService);

    void start(std::size_t numStones,
            std::shared_ptr<const HeurCalculator> heurCalculator,
            ComplexChecker checker,
            boost::optional<Array<bool>> excludeList = {},
            std::size_t startingIndex = 0);
    void wait(bool print);

    void synchronize(const std::function<void()>& function)
    {
        worker.synchronize(function);
    }
};

} // namespace sokoban

#endif /* SRC_SUBSTATUSFOREACH_HPP */
