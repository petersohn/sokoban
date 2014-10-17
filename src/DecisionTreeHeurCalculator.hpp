#ifndef DECISIONTREEHEURCALCULATOR_H_
#define DECISIONTREEHEURCALCULATOR_H_

#include "HeurCalculator.hpp"
#include "HeurInfo.hpp"
#include "Status/PseudoStatus.hpp"
#include "DecisionTree/DecisionTree.hpp"
#include "TimeMeter.hpp"
#include "Checker.hpp"
#include "SubStatusHeurCalculator.hpp"

class DecisionTreeHeurListFactory {
	typedef decisionTree::Node<PseudoStatus, int> NodeType;

	std::unique_ptr<NodeType> decisionTree_;

	static std::vector<Point> pointList(const Table& table);

	friend class Next;
	class Next {
		DecisionTreeHeurListFactory* owner_ = nullptr;
		std::size_t lastSize_ = 0;
		const NodeType::ValueList* heurList_ = nullptr;
		NodeType::ValueList::const_iterator iterator_;

	public:

		explicit Next(DecisionTreeHeurListFactory* owner):
			owner_(owner)
		{}

		const NodeType::Value* operator()(const PseudoStatus& pseudoStatus)
		{
			if (pseudoStatus.state().size() == 0) {
				return nullptr;
			}

			if (pseudoStatus.state().size() != lastSize_) {
				lastSize_ = pseudoStatus.state().size();
				heurList_ = &owner_->decisionTree_->get(pseudoStatus);
				iterator_ = heurList_->begin();
			}

			return iterator_ == heurList_->end() ? nullptr :
				(iterator_++)->get();
		}

	};

public:
	template <class HeurListType>
	DecisionTreeHeurListFactory(
			const Table& table,
			const HeurListType& heurList,
			const Checker::Ptr& checker,
			std::size_t maxDepth,
			std::size_t numThreads)
	{
		TimeMeter timeMeter;
		NodeType::ValueList convertedHeurList;
		convertedHeurList.reserve(heurList.size());
		boost::transform(heurList, std::back_inserter(convertedHeurList),
				[](const HeurInfo& heurInfo)
				{
					return std::make_shared<std::pair<PseudoStatus, int>>
							(PseudoStatus(heurInfo.first), heurInfo.second);
				});
		decisionTree_ = decisionTree::buildNode<PseudoStatus, int>(
				convertedHeurList,
				pointList(table),
				checker,
				maxDepth,
				numThreads);
		std::cerr << "Processor time: " <<
				timeMeter.processorTime() <<
				"\nReal time: " <<
				timeMeter.realTime() << std::endl;
	}

	Next operator()()
	{
		return Next{this};
	}

};

using DecisionTreeHeurCalculator = SubStatusHeurCalculator<DecisionTreeHeurListFactory>;


#endif /* DECISIONTREEHEURCALCULATOR_H_ */
