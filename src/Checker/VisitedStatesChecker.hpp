#ifndef VISITEDSTATESCHECKER_HPP
#define VISITEDSTATESCHECKER_HPP

#include "Checker/NodeChecker.hpp"
#include "Checker/VisitedStates.hpp"
#include "Node.hpp"

#include <memory>

class VisitedStatesChecker: public NodeChecker {
    VisitedStates visitedStates;
public:
	VisitedStatesChecker(VisitedStates visitedStates):
			visitedStates(std::move(visitedStates)) {
	}

	bool check(const Status& status, const Node& node) override
	{
		VisitedStateInput input(status, node.costFgv());
		return visitedStates.checkAndPush(input);
	}

	const char* errorMessage() const override
	{
		return "already visited";
	}
};

#endif // VISITEDSTATESCHECKER_HPP
