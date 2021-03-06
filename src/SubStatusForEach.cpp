#include "SubStatusForEach.hpp"

#include "Checker/ComplexCheckerBase.hpp"
#include "HeurCalculator/HeurCalculator.hpp"
#include "Status/State.hpp"
#include "Status/StatusUtils.hpp"
#include "Status/Table.hpp"

namespace sokoban {

SubStatusForEach::SubStatusForEach(
            const Table& table,
            const Action& action,
            MinDistance minDistance,
            MaxDistance maxDistance,
            ChokePointDistantNum chokePointDistantNum,
            Matrix<bool> chokePoints,
            WorkQueueLength workQueueLength,
            ReverseSearchMaxDepth reverseSearchMaxDepth,
            boost::asio::io_service& ioService):
        worker(ioService, workQueueLength.value()),
        table_(&table),
        action_(action),
        minDistance_(minDistance.value()),
        maxDistance_(maxDistance.value()),
        chokePointDistantNum_(chokePointDistantNum.value()),
        reverseSearchMaxDepth_(reverseSearchMaxDepth.value()),
        range_(matrixRange(table)),
        chokePoints_(std::move(chokePoints))
    {
    }

void SubStatusForEach::initIter(PointRange::iterator it, std::size_t stones,
        const State& state)
{
    if (!state.empty()) {
        if (!checkDistances(state, *it)) {
            return;
        }

        if (!processState(state, stones)) {
            return;
        }

        assert(stones > 0);
    }
    do {
        Point p = *it;
        if (!excludeList_[p] && !state[p]) {
            State state2(state);
            state2.addStone(p);
            initIter(it, stones - 1, state2);
        }
    } while (++it != range_.end());
}

bool SubStatusForEach::checkDistances(const State& state, Point p)
{
    if (maxDistance_ > 0 || minDistance_ > 0) {
        std::size_t numWrongDistance = 0;
        for (Point pp: state) {
            if (pp == p) {
                continue;
            }
            std::size_t xDistance = std::abs(p.x - pp.x);
            std::size_t yDistance = std::abs(p.y - pp.y);
            if (
                    (maxDistance_ > 0 && (xDistance > maxDistance_ ||
                        yDistance > maxDistance_)) ||
                    (minDistance_ > 0 && (xDistance < minDistance_ &&
                        yDistance < minDistance_))
                ) {
                if (chokePointDistantNum_ > 0 &&
                        (chokePoints_[p] || chokePoints_[pp])) {
                    if (++numWrongDistance > chokePointDistantNum_) {
                        return false;
                    }
                } else {
                    return false;
                }
            }
        }
    }

    return true;
}

bool SubStatusForEach::processState(const State& state, std::size_t stones)
{
    auto partitions = getPartitions(*table_, state, reverseSearchMaxDepth_);
    bool result = false;

    for (const Status& status: partitions) {
        float heur = heurCalculator_->calculateStatus(status);
        if (heur < 0) {
            continue;
        }
        if (!checkStatus(checker_, status)) {
            continue;
        }

        std::size_t index = index_++;
        if (stones == 0) {
            if (index >= startingIndex_) {
                // This will be accessed from another thread. Make sure that no
                // shared pointers that are potentially modified are shared
                // between the threads.
                auto copiedStatus = status.deepCopy();
                worker.addAction([this, copiedStatus, index]() {
                        action_(copiedStatus, index);
                    });
            }
        } else {
            result = true;
        }
    }

    return result;
}

void SubStatusForEach::start(std::size_t numStones,
            std::shared_ptr<const HeurCalculator> heurCalculator,
            ComplexChecker checker, boost::optional<Matrix<bool>> excludeList,
            std::size_t startingIndex)
{
    heurCalculator_ = std::move(heurCalculator);
    checker_ = std::move(checker);
    timeMeter_.reset();

    if (excludeList) {
        assert(excludeList->width() == table_->width());
        assert(excludeList->height() == table_->height());
        excludeList_ = std::move(*excludeList);
    } else {
        excludeList_.reset(table_->width(), table_->height(), false);
    }

    excludeList_[table_->destination()] = true;

    for (Point p: matrixRange(*table_)) {
        if (table_->wall(p)) {
            excludeList_[p] = true;
        }
    }

    index_ = 0;
    startingIndex_ = startingIndex;

    worker.startFilling();
    initIter(range_.begin(), numStones, State());
    worker.finishFilling();
}

void SubStatusForEach::wait(bool print)
{
    worker.wait(print);

    if (print) {
        std::cerr << "Iteration processor time: " <<
                timeMeter_.processorTime() <<
                "\nIteration time: " <<
                timeMeter_.realTime() << std::endl;
    }
}

} // namespace sokoban
