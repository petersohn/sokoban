#include "SolutionChecker.hpp"
#include "Status/Status.hpp"
#include "Dumper/DumperFunctions.hpp"
#include <fstream>
#include <boost/foreach.hpp>
#include <boost/format.hpp>

using std::endl;

bool SolutionChecker::isSuccessor(const Status& oldStatus, const Node& node)
{
	if (!oldStatus.reachable(node.from() - node.d())) {
		errorDump_ << "Push point not reachable." << endl;
		return false;
	}
	Point to(node.from() + node.d());
	size_t neededStones =  oldStatus.state().size();
	if (to == oldStatus.table().destination()) {
		--neededStones;
	}
	if (node.state().size() != neededStones) {
		errorDump_ << "Stone number mismatch." << "Needed: " << neededStones << ", found: " << node.state().size() << endl;
		return false;
	}
	for(Point  p: oldStatus.state()) {
		if (p != node.from() && !node.state()[p]) {
			errorDump_ << "Stone point mismatch: " << pointStr(p) << endl;
			return false;
		}
	}
	return true;
}

void SolutionChecker::printError(const Node::Ptr& oldNode, const Node::Ptr& newNode, const Status& status, const char* errorMessage)
{
	errorDump_ << errorMessage << "\n";
	if (oldNode) {
		dumpNode(errorDump_, status.table(), *oldNode, (boost::format("Old node (depth = %d)") % oldNode->depth()).str());
	}
	if (newNode) {
		dumpNode(errorDump_, status.table(), *newNode, (boost::format("New node (depth = %d)") % newNode->depth()).str());
	}
	dumpStatus(errorDump_, status, "Status");
	errorDump_ << "\n" << endl;
}

#define ERROR(msg) do { \
	result = false; \
	printError(oldNode, node, status, (msg)); \
} while (false)

bool SolutionChecker::checkResult(const Status& initialStatus, const std::deque<std::shared_ptr<Node> >& solution)
{
	Status status(initialStatus);
	bool result = true;
	Node::Ptr oldNode;
	int resultLength = solution.back()->cost();
	for(std::shared_ptr<Node> node: solution) {
		Point to(node->from() + node->d());
		if (std::abs(node->d().x) + std::abs(node->d().y) != 1) {
			ERROR("Invalid move.");
		}
		if (node->from().x < 0 || node->from().x >= static_cast<int>(status.width()) ||
				node->from().y < 0 || node->from().y >= static_cast<int>(status.height())) {
			printError({}, {}, status, "'from' point outside table.");
			return false;
		}
		if (to.x < 0 || to.x >= static_cast<int>(status.width()) ||
				to.y < 0 || to.y >= static_cast<int>(status.height())) {
			printError({}, {}, status, "'to' point outside table.");
			return false;
		}
		if (status.table().wall(to)) {
			ERROR("Wall found at 'to' point.");
		}
		if (status.table().wall(node->from())) {
			ERROR("Wall found at 'from' point.");
		}
		if (to != status.table().destination() && !node->state()[to]) {
			ERROR("Stone not found at 'to' point.");
		}
		if (node->state()[node->from()]) {
			ERROR("Stone found at 'from' point.");
		}
		if (oldNode && node->depth() != oldNode->depth() + 1) {
			ERROR("Error in node depth");
		}
		if (node->ancestor() != oldNode) {
			ERROR("Ancestor link error");
		}
		if (!isSuccessor(status, *node)) {
			ERROR("Not a successor");
		}
		if (node->costFgv() > resultLength) {
			ERROR("Invalid heuristic");
		}
		if (node->experimtntalCostFgv() != node->costFgv()) {
			heurDump_ << "Found plus heur: " <<
					node->costFgv() << " --> " <<
					node->experimtntalCostFgv() <<
					" (real=" << resultLength << ")" << std::endl;
			dumpStatus(heurDump_, status,
					node->experimtntalCostFgv() > resultLength ? "bad heur" : "good heur",
					&status.reachableArray());
		}
		status.set(*node);
		oldNode = node;
	}
	if (oldNode && oldNode->heur() != 0) {
		printError(Node::Ptr(), oldNode, status, "Last heur is not 0.");
		result = false;
	}
	if (!status.state().empty()) {
		printError(Node::Ptr(), Node::Ptr(), status, "Last status contains stones.");
		result = false;
	}
	return result;
}
