#ifndef BLOCKLISTGENERATOR_H
#define BLOCKLISTGENERATOR_H

#include "Solver.hpp"
#include "IndexedStatusList.hpp"
#include "Dumper/DumperFunctions.hpp"
#include "util/ThreadPool.hpp"
#include "Checker.hpp"
#include "HeurInfo.hpp"
#include <fstream>
#include <memory>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/date_time.hpp>

class BlockListGenerator {
private:
	struct IncrementInfo {
		HeurInfo heurInfo_;
		int	difference_;
		IncrementInfo(HeurInfo heurInfo, int difference):
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

	Solver::Ptr solver_;
	HeurCalculator::Ptr calculator_;
	HeurCalculator::Ptr incrementalCalculator_;
	Checker::Ptr checker_;
	IndexedStatusList<int> blockList_;
	IncrementList heurList_;
	const Table* table_;
	std::size_t numStones_;
	std::size_t maxDistance_;
	std::size_t maxHeurListSize_;
	std::size_t workQueueLength_;
	std::vector<CalculationInfoPtr> calculationInfos_;
	std::ofstream dump_;
	util::ThreadPool threadPool_;
	std::size_t numThreads_;

	std::deque<Node::Ptr> doCalculateBlockList(const Status& status);
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
	BlockListGenerator(Solver::Ptr solver,
			HeurCalculator::Ptr calculator, 
			Checker::Ptr checker, std::size_t numStones, 
			std::size_t maxDistance, std::size_t maxHeurListSize, 
			std::size_t workQueueLength, std::size_t numThreads);
	Checker::Ptr checker();
	HeurCalculator::Ptr vectorHeurCalculator();
	HeurCalculator::Ptr decisionTreeHeurCalculator(std::size_t maxDepth, bool useChecker);
	void init(const Table& table);
};

#endif /* BLOCKLISTGENERATOR_H */
