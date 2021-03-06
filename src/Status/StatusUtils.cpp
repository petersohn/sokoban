#include "Status/StatusUtils.hpp"
#include "Status/IsStatusPossible.hpp"
#include "Checker/ComplexCheckerBase.hpp"
#include "Checker/Checker.hpp"

namespace sokoban {

std::vector<Status> getPartitions(const Table& table, const State& state,
            std::size_t maxDepth)
{
    Matrix<bool> pointsToProcess(table.width(), table.height(), false);
    int numberOfPoints = 0;
    for (Point  p: matrixRange(table)) {
        if (!table.wall(p) && !state[p])
        {
            pointsToProcess[p] = true;
            ++numberOfPoints;
        } else
            pointsToProcess[p] = false;
    }
    std::vector<Status> result;
    auto searchRange = matrixRange(table);
    auto searchIterator = searchRange.begin();
    while (numberOfPoints > 0) {
        while (!pointsToProcess[*searchIterator]) {
            ++searchIterator;
            assert(searchIterator != searchRange.end());
        }
        Status status{table, state, *searchIterator};

        for (auto it = searchIterator; it != searchRange.end(); ++it) {
            if (pointsToProcess[*it] && status.reachable(*it))
            {
                pointsToProcess[*it] = false;
                --numberOfPoints;
            }
        }

        if (isStatusPossible(status, maxDepth)) {
            result.push_back(std::move(status));
        }
    }
    return result;
}

bool checkStatus(const ComplexChecker& checker, const Status& status)
{
    for (Point pp: status.state()) {
        if (!checker.check(status, pp)) {
            return false;
        }
    }
    return true;
}

bool checkState(const ComplexChecker& checker, const Table& table, const State& state)
{
    std::vector<Status> partitions = getPartitions(table, state, 0);
    for (const Status& status: partitions) {
        if (checkStatus(checker, status)) {
            return true;
        }
    }
    return false;
}

} // namespace sokoban
