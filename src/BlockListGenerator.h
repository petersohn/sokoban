#ifndef BLOCKLISTGENERATOR_H
#define BLOCKLISTGENERATOR_H

#include "Solver.h"
#include "IndexedStatusList.h"
#include "Dumper/DumperFunctions.h"
#include "ThreadPool.h"
#include "Checker.h"
#include "HeurInfo.h"
#include <fstream>
#include <memory>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/date_time.hpp>

class BlockListGenerator {
private:
	struct IncrementInfo {
		HeurInfoConstPtr heurInfo_;
		int	difference_;
		IncrementInfo(const HeurInfoConstPtr& heurInfo, int difference):
			heurInfo_(heurInfo),
			difference_(difference)
		{}
		IncrementInfo(HeurInfoConstPtr&& heurInfo, int difference):
			heurInfo_(std::move(heurInfo)),
			difference_(difference)
		{}
		IncrementInfo(const IncrementInfo& other):
			heurInfo_(other.heurInfo_),
			difference_(other.difference_)
		{}
		IncrementInfo(IncrementInfo&& other):
			heurInfo_(std::move(other.heurInfo_)),
			difference_(other.difference_)
		{}
		IncrementInfo& operator=(const IncrementInfo& other)
		{
			heurInfo_ = other.heurInfo_;
			difference_ = other.difference_;
			return *this;
		}
		IncrementInfo& operator=(IncrementInfo&& other)
		{
			heurInfo_ = std::move(other.heurInfo_);
			difference_ = other.difference_;
			return *this;
		}
		static const HeurInfoConstPtr& getHeurInfo(const IncrementInfo& incrementInfo) {
			return incrementInfo.heurInfo_;
		}
	};


	typedef std::vector<IncrementInfo> IncrementList;

	Solver::Ptr solver_;
	HeurCalculator::Ptr calculator_;
	HeurCalculator::Ptr incrementalCalculator_;
	Checker::Ptr checker_;
	std::shared_ptr<IndexedStatusList<int>> blockList_;
	IncrementList heurList_;
	FixedTable::Ptr table_;
	int numStones_;
	int maxDistance_;
	int maxHeurListSize_;
	std::ofstream dump_;
	MutexType dumpMutex_;
	ThreadPool threadPool_;
	int numThreads_;

	std::deque<Node::Ptr> doCalculateBlockList(const Status& status);
	void calculateBlockList(const Status& status);
	void calculateHeurList(const Status& status);
	void dumpStatus(const Status &status, const Point *p, const std::string &title) {
		boost::lock_guard<MutexType> lck(dumpMutex_);
		if (!dump_.good())
			return;
		if (p) {
			Array<bool> hl = status.reachableArray();
			hl[*p] = true;
			::dumpStatus(dump_, status, title, &hl);
		} else {
			::dumpStatus(dump_, status, title, &status.reachableArray());
		}
	}
public:
	BlockListGenerator(Solver::Ptr solver,
			HeurCalculator::Ptr calculator, Checker::Ptr checker, int numStones,
			int maxDistance, int maxHeurListSize, int numThreads);
	Checker::Ptr checker();
	HeurCalculator::Ptr vectorHeurCalculator();
	HeurCalculator::Ptr decisionTreeHeurCalculator(int maxDepth, bool useChecker);
	void init(const FixedTable::Ptr& table);
};

#endif /* BLOCKLISTGENERATOR_H */
