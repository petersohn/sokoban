#ifndef DECISIONTREEHEURCALCULATOR_H_
#define DECISIONTREEHEURCALCULATOR_H_

#include "HeurCalculator.h"
#include "IndexedStatusList.h"
#include "HeurInfo.h"
#include "Status/PseudoStatus.h"
#include "DecisionTree/DecisionTree.h"
#include "TimeMeter.h"
#include "Checker.h"

class DecisionTreeHeurCalculator: public HeurCalculator {
	typedef decisionTree::Node<PseudoStatus, int> NodeType;

	HeurCalculator::Ptr baseCalculator_;
	FixedTable::Ptr table_;
	std::unique_ptr<NodeType> decisionTree_;

	static std::vector<Point> pointList(const FixedTable::Ptr& table);
public:
	template <class HeurListType>
	DecisionTreeHeurCalculator(
			const HeurCalculator::Ptr& baseCalculator,
			const HeurListType& heurList,
			const Checker::Ptr checker,
			FixedTable::Ptr table,
			int maxDepth,
			int numThreads):
				baseCalculator_(baseCalculator),
				table_(table)
	{
		TimeMeter timeMeter;
		NodeType::ValueList convertedHeurList;
		convertedHeurList.reserve(heurList.size());
		boost::transform(heurList, std::back_inserter(convertedHeurList),
				[](const HeurInfoConstPtr& heurInfo)
				{
					return std::make_shared<std::pair<PseudoStatus, int>>
							(PseudoStatus(heurInfo->first), heurInfo->second);
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
	virtual int calculateStone(const Status &status, Point p);
	virtual int calculateStatus(const Status &status,
			const MoveDescriptor* /*moveDescriptor*/,
			const std::shared_ptr<Node>& ancestor);
};



#endif /* DECISIONTREEHEURCALCULATOR_H_ */
