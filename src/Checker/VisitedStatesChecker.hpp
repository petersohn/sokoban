#ifndef VISITEDSTATESCHECKER_HPP
#define VISITEDSTATESCHECKER_HPP

#include "Checker/NodeChecker.hpp"
#include "VisitedStates.hpp"
#include "Node.hpp"

#include <memory>

class VisitedStatesChecker: public NodeChecker {
    std::shared_ptr<VisitedStates> visitedStates;
public:
	VisitedStatesChecker(std::shared_ptr<VisitedStates> visitedStates):
			visitedStates(std::move(visitedStates)) {
	}

	bool check(const Status& status, const Node& node) const override
	{
		VisitedStateInput vsi(status, node.costFgv());
		return visitedStates->checkAndPush(vsi);
	}

	const char* errorMessage() const override
	{
		return "already visited";
	}
};

#endif // VISITEDSTATESCHECKER_HPP
