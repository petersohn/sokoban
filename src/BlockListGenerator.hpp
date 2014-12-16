#ifndef BLOCKLISTGENERATOR_H
#define BLOCKLISTGENERATOR_H

#include "Solver.hpp"
#include "IndexedStatusList.hpp"
#include "Dumper/DumperFunctions.hpp"
#include "util/ThreadPool.hpp"
#include "Checker.hpp"
#include "ComplexChecker.hpp"
#include "HeurInfo.hpp"
#include "Options.hpp"
#include <fstream>
#include <memory>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/date_time.hpp>

class HeurCalculator;

class BlockListGenerator {
private:
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
	//std::size_t numThreads_;

	std::deque<std::shared_ptr<Node>> doCalculateBlockList(const Status& status);
	void calculateBlockList(const Status& status);
	void calculateHeurList(const Status& status);
	void dumpStatus(const Status &status, const Point *p, const std::string &title) {
		const std::size_t* threadId = util::ThreadPool::getCurrentThreadId();
		std::ostream* dump = &dump_;
		if (threadId) {
			dump = &calculationInfos_[*threadId]->dump_;
		}
		if (p) {
			Array<bool> hl = status.reachableArray();
			hl[*p] = true;
			::dumpStatus(*dump, status, title, &hl);
		} else {
			::dumpStatus(*dump, status, title, &status.reachableArray());
		}
	}
public:
	BlockListGenerator(std::unique_ptr<const Solver> solver, std::shared_ptr<const HeurCalculator> calculator,
			ComplexChecker checker, const Options& options);
	Checker::Ptr checker();
	std::shared_ptr<const HeurCalculator> vectorHeurCalculator(float heurMultiplier);
	std::shared_ptr<const HeurCalculator> decisionTreeHeurCalculator(std::size_t maxDepth, 
			bool useChecker, float heurMultiplier);
	void init(const Table& table);
};

#endif /* BLOCKLISTGENERATOR_H */
