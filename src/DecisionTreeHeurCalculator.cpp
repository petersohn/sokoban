#include "DecisionTreeHeurCalculator.hpp"

std::vector<Point>
DecisionTreeHeurListFactory::pointList(const Table& table)
{
	std::vector<Point> result;
	for (Point  p: arrayRange(table)) {
		if (!table.wall(p)) {
			result.push_back(p);
		}
	}
	return result;
}

const DecisionTreeHeurListFactory::NodeType::Value*
DecisionTreeHeurListFactory::Next::operator()(const PseudoStatus& pseudoStatus)
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
