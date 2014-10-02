#ifndef SOLUTIONCHECKER_H_
#define SOLUTIONCHECKER_H_

#include <queue>
#include <memory>
#include <ostream>
#include "Node.hpp"

class Status;
class Node;

class SolutionChecker {
	std::ostream& errorDump_;
	std::ostream& heurDump_;

	bool isSuccessor(const Status& oldStatus, const Node& node);
	void printError(const Node::Ptr& oldNode, const Node::Ptr& newNode, const Status& status, const char* errorMessage);
public:
	SolutionChecker(
			std::ostream& errorDump,
			std::ostream& heurDump):
				errorDump_(errorDump),
				heurDump_(heurDump)
	{}
	bool checkResult(const Status& initialStatus, const std::deque<std::shared_ptr<Node> >& solution);
};



#endif /* SOLUTIONCHECKER_H_ */