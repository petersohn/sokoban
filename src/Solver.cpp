#include "Solver.h"
#include "Table.h"
#include "State.h"
#include "Status.h"
#include "Node.h"
#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <deque>
#include <queue>
#include <algorithm>
#include <boost/format.hpp>

class InternalSolver {
	HeurCalculator::Ptr calculator_;
	FixedTable::Ptr table_;
	std::multiset<VisitedState> visitedStates_;
	std::priority_queue<Node::Ptr> queue_;

	std::ofstream dumpFile_;
	bool enableDump_;
	bool enableLog_;
	int expandedNodes_;
	int maxDepth_;

	void expandNodes(const VisitedState &state, Node::Ptr base);
	void expandNode(const VisitedState &state, int stone,
			const Point &d, Node::Ptr base);
	void checkStone(const Status &status, int stone);
	bool stoneMovable(const Status &status, int stone, set<int> &volt);
	void addVisitedState(const VisitedState &state);
	bool statusVisited(const Status &status);
	void pushQueue(Node::Ptr node);
	Node::Ptr popQueue();
public:
	explicit InternalSolver(bool enableLog, bool enableDump);

	std::vector<Node::Ptr> solve(const Status &status, HeurCalculator::Ptr calculator);
};

inline explicit InternalSolver::InternalSolver(bool enableLog, bool enableDump):
		enableLog_(enableLog),
		enableDump_(enableDump)
{
	if (enableDump_) {
		static int counter = 0;
		boost::format filename("dump%1%.dump",
				std::ofstream::out | std::ofstream::trunc);
		if (++counter > 1)
			filename % counter;
		else
			filename % "";
		dumpFile_.open(filename.str());
	}
}

inline std::deque<Node::Ptr> InternalSolver::solve(const Status &status,
		HeurCalculator::Ptr calculator)
{
	calculator_ = calculator;
	table_ = status.tablePtr();
	visitedStates_.clear();
	expandedNodes_ = 0;
	maxDepth_ = 0;
	Node::Ptr currentNode;
	VisitedState vs(status.state());
	do
	{
		addVisitedState(vs);
		expandNodes(vs, currentNode);
		currentNode = popQueue();
		if (currentNode.get() == NULL)
			return std::vector<Node::Ptr>();
		vs = VisitedState(currentNode);
	} while (currentNode->heur() > 0);
	return currentNode->pathToRoot();
}


void InternalSolver::expandNodes(const VisitedState &state, Node::Ptr base) {
	if (pushStones())
		return;
	for (int i = 0; i < state.size(); ++i)
	{
		if (state[i] == destination)
			continue;
		expandNode(state, i, p10, base);
		expandNode(state, i, pm10, base);
		expandNode(state, i, p01, base);
		expandNode(state, i, p0m1, base);
	}
}

void InternalSolver::expandNode(const VisitedState &state, int stone,
		const Point &d, Node::Ptr base) {
	Status status(table_, state);
	Point p = state[stone];
	Point pd = p+d, pmd = p-d;
	if (pmd.x >= 0 && pmd.x < status.width() &&
			pmd.y >= 0 && pmd.y < status.height() &&
			status.value(pd) == ftFloor && status.reachable(pmd) &&
			calculator_->calculateStone(status, pd) >= 0
			)
	{
		if (status.moveStone(stone, pd) && !stateVisited(status) &&
				(status.state()[stone] == table_.destination() ||
					checkStone(status, stone)))
		{
			Node::Ptr node(Node::create(status.state(), stone, d,
					base, 1, calculator_->calculateStatus(status)));
			pushQueue(node);
			addVisitedState(status.state());
			if (enableDump)
				dumpNode(dumpFile_, table_, node, "Felvéve");
			maxDepth_ = std::max(node->depth(), maxDepth_);
			if (enableLog && ++expandedNodes_ % 10000 == 0)
				cerr << boost::format("%d (%d, %d [%d])") %
					expandedNodes_ % queue.size() %
					node->depth() % maxDepth_ << endl;
		}
	}
}

void InternalSolver::checkStone(const Status &status, int stone) {
	std::set<int> volt;
	bool result = stoneMovable(status, stone, volt) && checkCorridors(status, stone);
	return result;
}

bool InternalSolver::stoneMovable(const Status &status, int stone, set<int> &volt)
{
	Point p = status.state()[stone];
	volt.insert(stone);
	int count = 0;
	if (isMovable(status, p+p10, volt, count) &&
			isMovable(status, p+pm10, volt, count) && count > 0)
		return true;
	count = 0;
	return (isMovable(status, p+p01, volt, count) &&
			isMovable(status, p+p0m1, volt, count) && count > 0);
}

bool InternalSolver::isMovable(const Status &status, const Point & p, set<int> &volt, int &count)
{
	if (status.value(p) == ftFloor)
	{
		if (calculator_->calculateStone(status, p) >= 0)
			++count;
		return true;
	}
	if (status.value(p) == ftWall)
		return false;
	if (volt.count(status.stoneAt(p)) != 0)
		return false;
	return stoneMovable(status, stoneAt[p], volt);
}

void InternalSolver::addVisitedState(const VisitedState &state) {
	visitedStates_.insert(state);
}

bool InternalSolver::statusVisited(const Status &status) {
	std::pair<std::multiset<VisitedState>::iterator,
		std::multiset<VisitedState>::iterator> found =
			visitedStates.equal_range(stones);
	if (found.first != found.second)
	{
		Array<bool> reach(width, height, false);
		floodFill(status, status.state().currentPos(), reach);
		for (std::multiset<VisitedState>::iterator it = found.first;
				it != found.second; ++it) {
			if (reach[it->currentPos()] && *it == status.state())
				return true;
		}
	}
	return false;
}

void InternalSolver::pushQueue(Node::Ptr node) {
	queue_.push(node);
}

Node::Ptr InternalSolver::popQueue() {
	Node::Ptr result = queue_.top();
	queue_.pop();
	return result;
}




std::deque<Node::Ptr> Solver::solve(const Status &status,
		HeurCalculator::Ptr calculator, bool enableLog, bool enableDump)
{
	InternalSolver solver;
	solver.solve(status, calculator);
}

