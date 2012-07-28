#include "DecisionTree/OptimalSplitting.h"
#include "DecisionTreeHeurCalculator.h"
#include "Dumper/DumperFunctions.h"
#include "Node.h"
#include "VisitedStateInfo.h"
#include "AnnotatedFunction.h"
#include <boost/range/algorithm.hpp>
#include <boost/range/numeric.hpp>
#include <functional>

int DecisionTreeHeurCalculator::calculateStone(const Status &status, const Point &p)
{
	assert(status.tablePtr() == table_);
	return baseCalculator_->calculateStone(status, p);
}

int DecisionTreeHeurCalculator::calculateStatus(const Status &status,
			const std::shared_ptr<Node>& ancestor)
{
	assert(status.tablePtr() == table_);
	MockStatus mockStatus(status);
	int result = 0;
	while (true) {
		const std::pair<MockStatus, int>* subset = decisionTree_->get(mockStatus);
		if (!subset || !isSubStatus(subset->first, mockStatus)) {
			break;
		}
		result += subset->second;
		for (const Point& p: subset->first.state()) {
			mockStatus.state().removeStone(p);
		}
	}
	for (const Point& p: mockStatus.state()) {
		result += baseCalculator_->calculateStone(status, p);
	}
	if (ancestor) {
		return std::max(result, ancestor->heur() - 1);
	}
	return result;
}

namespace {

typedef AnnotatedFunction<bool(const MockStatus&)> Functor;
typedef std::vector<Functor> FunctorList;

FunctorList functorList(const FixedTable::Ptr& table)
{
	FunctorList result;
	Point p;
	for (p.y = 0; p.y < table->get().height(); ++p.y) {
		for (p.x = 0; p.x < table->get().width(); ++p.x) {
			if (!table->get().wall(p)) {
				result.push_back([p](const MockStatus& status)
						{
							return status.state()[p];
						} );
				result.back().name("hasStone" + pointStr(p));
				result.push_back([p](const MockStatus& status)
						{
							return status.reachable(p);
						} );
				result.back().name("reachable" + pointStr(p));
			}
		}
	}
	return result;
}

std::pair<MockStatus, int>
convertHeurInfo(const std::unique_ptr<HeurInfo>& heurInfo)
{
	return std::make_pair(MockStatus(heurInfo->first), heurInfo->second);
}

}

DecisionTreeHeurCalculator::DecisionTreeHeurCalculator(
		const HeurCalculator::Ptr& baseCalculator,
		const HeurListPtr& heurList,
		FixedTable::Ptr table,
		int level):
			baseCalculator_(baseCalculator),
			table_(table)
{
	std::vector<std::pair<MockStatus, int>> convertedHeurList;
	convertedHeurList.reserve(heurList->size());
	boost::transform(*heurList, std::back_inserter(convertedHeurList), convertHeurInfo);
	decisionTree_ = decisionTree::buildNode(
			convertedHeurList,
			functorList(table),
			decisionTree::OptimalSplitting());
}
