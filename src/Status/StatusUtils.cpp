#include "Status/StatusUtils.hpp"
#include "ComplexChecker.hpp"
#include "Status/IsStatusPossible.hpp"

std::vector<Status> getPartitions(const Table& table, const State& state,
			std::size_t maxDepth)
{
	Array<bool> pointsToProcess(table.width(), table.height(), false);
	int numberOfPoints = 0;
	for (Point  p: arrayRange(table)) {
		if (!table.wall(p) && !state[p])
		{
			pointsToProcess[p] = true;
			++numberOfPoints;
		} else
			pointsToProcess[p] = false;
	}
	std::vector<Status> result;
	while (numberOfPoints > 0) {
		Point foundPoint;
		for (Point  p: arrayRange(table)) {
			if (pointsToProcess[p]) {
				foundPoint = p;
				break;
			}
		}
		Status status{table, state, foundPoint};

		for (Point  p: arrayRange(table)) {
			if (status.reachable(p) && pointsToProcess[p])
			{
				pointsToProcess[p] = false;
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




