#include "ChokePointFinder.hpp"
#include "Status/Table.hpp"
#include "Status/Point.hpp"
#include "Status/Status.hpp"
#include "Status/StatusUtils.hpp"
#include "TableIterator.hpp"
#include "HeurCalculator.hpp"
#include "ComplexChecker.hpp"
#include "util/ThreadPool.hpp"

namespace {

class ChokePointFinder {
	const Table* table;
	Options options;
	std::shared_ptr<const HeurCalculator> calculator;
	ComplexChecker checker;

	using CalculationInfo = Array<bool>;

	std::vector<CalculationInfo> calculationInfos;

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
		calculator(std::move(calculator)), checker(std::move(checker))
	{}

	void find(bool print)
	{
		if (print) {
			std::cerr << "Calculating choke points..." << std::endl;
		}

		util::ThreadPool threadPool{options.numThreads_};
		TableIterator tableIterator(*table,
				std::bind(&ChokePointFinder::checkChokePoint, this, std::placeholders::_1),
				TableIterator::MinDistance{options.chokePointDistance_},
				TableIterator::MaxDistance{0},
				TableIterator::WorkQueueLength{options.workQueueLength_},
				TableIterator::ReverseSearchMaxDepth{options.reverseSearchMaxDepth_},
				threadPool.getIoService());

		calculationInfos.resize(options.numThreads_, CalculationInfo{
				table->width(), table->height(), false});
		{
			util::ThreadPoolRunner runner{threadPool};
			tableIterator.start(options.chokePointNum_, calculator, checker);
			tableIterator.wait(print);
		}
	}

	std::vector<Point> get()
	{
		std::vector<Point> result;

		for (Point p: arrayRange(*table)) {
			if (std::any_of(calculationInfos.begin(), calculationInfos.end(),
						[p](const CalculationInfo& info) { return info[p]; })) {
				result.push_back(p);
			}
		}

		return result;
	}
};

}

std::vector<Point> findChokePoints(const Table& table, Options options,
			std::shared_ptr<const HeurCalculator> calculator,
			ComplexChecker checker, bool print)
{
	ChokePointFinder finder(table, std::move(options),
			std::move(calculator), std::move(checker));
	finder.find(print);
	return finder.get();
}

