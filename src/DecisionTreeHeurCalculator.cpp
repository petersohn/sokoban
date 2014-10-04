#include "DecisionTreeHeurCalculator.hpp"
#include "Dumper/DumperFunctions.hpp"
#include "Node.hpp"
#include "VisitedStateInfo.hpp"
#include "AnnotatedFunction.hpp"
#include "Status/StatusUtils.hpp"
#include <boost/range/algorithm.hpp>
#include <boost/range/numeric.hpp>
#include <functional>
#include <iostream>

int DecisionTreeHeurCalculator::calculateStone(const Status &status, Point p)
{
	assert(status.tablePtr() == table_);
	return baseCalculator_->calculateStone(status, p);
}

int DecisionTreeHeurCalculator::calculateStatus(
		const Status &status,
		const MoveDescriptor* /*moveDescriptor*/,
		const std::shared_ptr<Node>& ancestor)
{
	assert(status.tablePtr() == table_);

	PseudoStatus pseudoStatus(status);
	int result = 0;
	bool found;
	do {
		found = false;
		const NodeType::ValueList& heurList =
			decisionTree_->get(pseudoStatus);
		for (const NodeType::ValuePtr& subset: heurList) {
			if (isSubStatus(subset->first, pseudoStatus)) {
				result += subset->second;
				for (Point  p: subset->first.state()) {
					pseudoStatus.state().removeStone(p);
				}
				found = true;
				break;
			}
		}
	} while (found);
	for (Point  p: pseudoStatus.state()) {
		result += baseCalculator_->calculateStone(status, p);
	}
	if (ancestor) {
		return std::max(result, ancestor->heur() - 1);
	}
	return result;
}

std::vector<Point>
DecisionTreeHeurCalculator::pointList(const FixedTable::Ptr& table)
{
	std::vector<Point> result;
	for (Point  p: arrayRange(table->get())) {
		if (!table->get().wall(p)) {
			result.push_back(p);
		}
	}
	return result;
}

