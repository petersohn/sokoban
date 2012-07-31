#ifndef DECISIONTREEHEURCALCULATOR_H_
#define DECISIONTREEHEURCALCULATOR_H_

#include "HeurCalculator.h"
#include "IndexedStatusList.h"
#include "HeurInfo.h"
#include "Status/MockStatus.h"
#include "DecisionTree/DecisionTree.h"

class DecisionTreeHeurCalculator: public HeurCalculator {
	typedef decisionTree::Node<MockStatus, int> NodeType;

	HeurCalculator::Ptr baseCalculator_;
	FixedTable::Ptr table_;
	std::unique_ptr<NodeType> decisionTree_;
public:
	DecisionTreeHeurCalculator(
			const HeurCalculator::Ptr& baseCalculator,
			const HeurListPtr& heurList,
			FixedTable::Ptr table,
			int maxDepth);
	virtual int calculateStone(const Status &status, const Point &p);
	virtual int calculateStatus(const Status &status,
			const std::shared_ptr<Node>& ancestor);
};



#endif /* DECISIONTREEHEURCALCULATOR_H_ */
