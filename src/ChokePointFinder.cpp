#include "ChokePointFinder.hpp"
#include "Status/Table.hpp"
#include "Status/Point.hpp"
#include "Status/Status.hpp"
#include "Status/StatusUtils.hpp"
#include "SubStatusForEach.hpp"
#include "HeurCalculator.hpp"
#include "ComplexChecker.hpp"
#include "util/ThreadPool.hpp"
#include <algorithm>

namespace {

class ChokePointFinder {
	const Table* table;
	Options options;
	std::shared_ptr<const HeurCalculator> calculator;
	ComplexChecker checker;

	using CalculationInfo = Array<bool>;

	std::vector<CalculationInfo> calculationInfos;
	CalculationInfo result;

	void checkChokePoint(const Status& status)
	{
		auto partitions = getPartitions(*table, status.state(),
				options.reverseSearchMaxDepth_);
		if (partitions.size() > 1) {
			auto threadId = *util::ThreadPool::getCurrentThreadId();
			for (Point p: status.state()) {
				calculationInfos[threadId][p] = true;
			}
		}
	}
public:
	ChokePointFinder(const Table& table, Options options,
			std::shared_ptr<const HeurCalculator> calculator,
			ComplexChecker checker):
		table(&table), options(std::move(options)),
		calculator(std::move(calculator)), checker(std::move(checker)),
		result{table.width(), table.height(), false}
	{}

	void find(bool print)
	{
		if (print) {
			std::cerr << "Calculating choke points..." << std::endl;
		}

		util::ThreadPool threadPool{options.numThreads_};
		SubStatusForEach tableIterator(*table,
				std::bind(&ChokePointFinder::checkChokePoint, this, std::placeholders::_1),
				SubStatusForEach::MinDistance{options.chokePointDistance_},
				SubStatusForEach::MaxDistance{0},
				SubStatusForEach::ChokePointDistantNum{0}, {},
				SubStatusForEach::WorkQueueLength{options.workQueueLength_},
				SubStatusForEach::ReverseSearchMaxDepth{options.reverseSearchMaxDepth_},
				threadPool.getIoService());

		calculationInfos.resize(options.numThreads_, CalculationInfo{
				table->width(), table->height(), false});
		for (std::size_t numStones = 1; numStones <= options.chokePointNum_;
				++numStones) {
			if (print) {
				std::cerr << "Stones = " << numStones << std::endl;
			}

			{
				util::ThreadPoolRunner runner{threadPool};
				tableIterator.start(numStones, calculator, checker, result);
				tableIterator.wait(print);
			}

			for (Point p: arrayRange(*table)) {
				if (std::any_of(calculationInfos.begin(), calculationInfos.end(),
							[p](const CalculationInfo& info) { return info[p]; })) {
					result[p] = true;
				}
			}

		}
	}

	CalculationInfo get()
	{
		return result;
	}
};

}

Array<bool> findChokePoints(const Table& table, Options options,
			std::shared_ptr<const HeurCalculator> calculator,
			ComplexChecker checker, bool print)
{
	ChokePointFinder finder(table, std::move(options),
			std::move(calculator), std::move(checker));
	finder.find(print);
	return finder.get();
}

