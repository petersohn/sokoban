#ifndef EXPANDEDNODELIMITER_HPP
#define EXPANDEDNODELIMITER_HPP

#include "Checker/NodeChecker.hpp"
#include "Node.hpp"

class ExpandedNodeLimiter: public NodeChecker {
	std::size_t& expandedNodes;
	std::size_t expandedNodeLimit;
public:
	ExpandedNodeLimiter(std::size_t& expandedNodes,
			std::size_t expandedNodeLimit):
			expandedNodes(expandedNodes),
			expandedNodeLimit(expandedNodeLimit) {}

	bool check(const Status& /*status*/, const Node& node) const override
	{
		return expandedNodes < expandedNodeLimit || node.heur() == 0;
	}

	const char* errorMessage() const override
	{
		return "node limit exceeded";
	}
};


#endif // EXPANDEDNODELIMITER_HPP
