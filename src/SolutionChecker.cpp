#include "SolutionChecker.h"
#include "Node.h"
#include "Status.h"
#include "DumperFunctions.h"
#include "Common.h"
#include <iostream>
#include <boost/foreach.hpp>
#include <boost/format.hpp>

namespace {

using std::cerr;
using std::endl;

bool isSuccessor(const Status& oldStatus, const Node& node)
{
	if (!oldStatus.reachable(node.from() - node.d())) {
		cerr << "Push point not reachable." << endl;
		return false;
	}
	Point to(node.from() + node.d());
	size_t neededStones =  oldStatus.state().size();
	if (to == oldStatus.table().destination()) {
		--neededStones;
	}
	if (node.state().size() != neededStones) {
		cerr << "Stone number mismatch." << "Needed: " << neededStones << ", found: " << node.state().size() << endl;
		return false;
	}
	BOOST_FOREACH(const Point& p, oldStatus.state()) {
		if (p != node.from() && !node.state()[p]) {
			cerr << "Stone point mismatch: " << pointStr(p) << endl;
			return false;
		}
	}
	return true;
}

void printError(const Node::Ptr& oldNode, const Node::Ptr& newNode, const Status& status, const char* errorMessage)
{
	cerr << errorMessage << "\n";
	if (oldNode) {
		dumpNode(cerr, status.tablePtr(), *oldNode, (boost::format("Old node (depth = %d)") % oldNode->depth()).str());
	}
	if (newNode) {
		dumpNode(cerr, status.tablePtr(), *newNode, (boost::format("New node (depth = %d)") % newNode->depth()).str());
	}
	dumpStatus(cerr, status, "Status");
	cerr << "\n" << endl;
}

}

bool checkResult(const Status& initialStatus, const std::deque<boost::shared_ptr<Node> >& solution)
{
	Status status(initialStatus);
	bool result = true;
	Node::Ptr oldNode;
	BOOST_FOREACH(boost::shared_ptr<Node> node, solution) {
		Point to(node->from() + node->d());
		if (to != status.table().destination() && !node->state()[to]) {
			result = false;
			printError(oldNode, node, status, "Stone not found at 'to' point.");
		}
		if (node->state()[node->from()]) {
			result = false;
			printError(oldNode, node, status, "Stone found at 'from' point.");
		}
		if (oldNode && node->depth() != oldNode->depth() + 1) {
			result = false;
			printError(oldNode, node, status, "Error in node depth");
		}
		if (node->ansector() != oldNode) {
			result = false;
			printError(oldNode, node, status, "Ansector link error");
		}
		if (!isSuccessor(status, *node)) {
			result = false;
			printError(oldNode, node, status, "Not a successor");
		}
		status.set(*node);
		oldNode = node;
	}
	return result;
}