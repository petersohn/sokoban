#include "DecisionTreeHeurCalculator.h"
#include "Dumper/DumperFunctions.h"
#include "Node.h"
#include "VisitedStateInfo.h"
#include "AnnotatedFunction.h"
#include <boost/range/algorithm.hpp>
#include <boost/range/numeric.hpp>
#include <functional>
#include <iostream>

int DecisionTreeHeurCalculator::calculateStone(const Status &status, const Point &p)
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

	MockStatus mockStatus(status);
	int result = 0;
	bool found;
	do {
		found = false;
		const NodeType::ValueList& heurList =
			decisionTree_->get(mockStatus);
		for (const NodeType::ValuePtr& subset: heurList) {
			if (isSubStatus(subset->first, mockStatus)) {
				result += subset->second;
				for (const Point& p: subset->first.state()) {
					mockStatus.state().removeStone(p);
				}
				found = true;
				break;
			}
		}
	} while (found);
	for (const Point& p: mockStatus.state()) {
		result += baseCalculator_->calculateStone(status, p);
	}
	if (ancestor) {
		return std::max(result, ancestor->heur() - 1);
	}
	return result;
}

DecisionTreeHeurCalculator::FunctorPtrList
DecisionTreeHeurCalculator::functorList(const FixedTable::Ptr& table)
{
	FunctorPtrList result;
	Point p;
	for (p.y = 0; p.y < table->get().height(); ++p.y) {
		for (p.x = 0; p.x < table->get().width(); ++p.x) {
			if (!table->get().wall(p)) {
				result.push_back(std::make_shared<Functor>(
						[p](const MockStatus& status)
						{
							return status.state()[p];
						} ));
				result.back()->name("hasStone" + pointStr(p));
			}
		}
	}
	return result;
}

