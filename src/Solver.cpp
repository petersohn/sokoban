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
	bool isMovable(const Status &status, const Point & p,
			set<int> &volt, int &count);
	bool checkCorridorEnding(const Status &status,
			const Point &p0, const Point &side);
	bool checkCorridors(const Status &status, int stone);
	bool pushStones(const Status &status);
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
/*	if (pushStones(Status(state)))
		return;*/
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
				dumpNode(dumpFile_, table_, node, "Added");
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

bool InternalSolver::isMovable(const Status &status, const Point & p,
		set<int> &volt, int &count)
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

bool InternalSolver::checkCorridors(const Status &status, int stone) {
	Point p0 = status.state()[stone];
	bool kell[3][3];
	for (int x = 0; x < 3; x++)
		for (int y = 0; y < 3; y++)
			kell[x][y] = true;

	for (int x = 0; x < 3; x++)
		for (int y = 0; y < 3; y++) {
			Point p = p0 + Point(x - 1, y - 1);
			if (!kell[x][y] || status.fieldAt(p) != ftFloor || status.reachable(p))
				continue;
			Array<bool> reach(width, height, false);
			MinMax minmax;
			floodFill(status, p, reach, NULL, &minmax);
			if (!reach[destination]) {
				if (minmax.minX == minmax.maxX && minmax.minY == minmax.maxY) {
					if (!checkCorridorEnding(status,
								Point(minmax.minX, minmax.minY - 1), Point::p10) &&
						!checkCorridorEnding(status,
								Point(minmax.minX, minmax.maxY + 1), Point::p10) &&
						!checkCorridorEnding(status,
								Point(minmax.minX - 1, minmax.minY), Point::p01) &&
						!checkCorridorEnding(status,
								Point(minmax.maxX + 1, minmax.minY), Point::p01)) {
//						cerr << "Bing!" << endl;
						if (enableDump) {
							dumpStatus(dump, status, "1x1 corridor found", &reach);
						}
						return false;
					}
				} else
				if (minmax.minX == minmax.maxX) {
					if (!checkCorridorEnding(status,
								Point(minmax.minX, minmax.minY - 1), Point::p10) &&
						!checkCorridorEnding(status,
								Point(minmax.minX, minmax.maxY + 1), Point::p10)) {
						if (enableDump) {
							dumpStatus(dump, status, "Vertical corridor found", &reach);
						}
						return false;
					}
				} else
				if (minmax.minY == minmax.maxY) {
					if (!checkCorridorEnding(status,
								Point(minmax.minX - 1, minmax.minY), Point::p01) &&
						!checkCorridorEnding(status,
								Point(minmax.maxX + 1, minmax.minY), Point::p01)) {
						if (enableDump) {
							dumpStatus(dump, status, "Horizontal corridor found", &reach);
						}
						return false;
					}
				}
			}
			for (int xx = 0; xx < 3; xx++)
				for (int yy = 0; yy < 3; yy++) {
					Point pp = p0 + Point(xx - 1, yy - 1);
					if (reach[pp])
						kell[xx][yy] = false;
				}
		}
	return true;
}

bool InternalSolver::checkCorridorEnding(const Status &status,
		const Point &p0, const Point &side) {
	Point p1 = p0 + side;
	Point pm1 = p0 - side;
/*	cerr << boost::format("Corr (%d): (%d, %d): %d %d; (%d, %d): %d %d. "
			"Pos: (%d, %d)") % fieldAt(p0)
			% p1.x % p1.y % fieldAt(p1) % heurAt(p1)
			% pm1.x % pm1.y % fieldAt(pm1) % heurAt(pm1)
			% stones.currentPos.x % stones.currentPos.y << endl;*/
	return status.value(p0) != ftWall &&
			status.value(p1) == ftFloor &&
			status.value(pm1) == ftFloor &&
			(calculator_.heurAt(status, p1) || calculator_.heurAt(status, pm1));
}

bool InternalSolver::pushStones(const Status &status)
{
/*	Array<bool> destReachable(width, height, false);
	std::deque<VisitedState> pushList;
	bool touched;
	bool touched2 = false;
	Node::Ptr node;
	do {
		touched = false;
		std::deque<int> destBorder;
		floodFill(status, destination, destReachable, &destBorder);
		std::deque<int> border = intersect(status.border(), destBorder);
		for (std::deque<int>::const_iterator it = border.begin();
				it != border.end(); ++it) {
			node = pushStone(status, *it);
			if (tmp.get() != NULL) {
				maxDepth_ = std::max(state->depth, maxDepth);
				touched = true;
			}
		}
		if (touched)
			touched2 = true;

	} while (touched);
	if (touched2 && !stateVisited())
	{
		visitedStates.insert(pushList.begin(), pushList.end());
		queue.push(state);
		return true;
	}
	else
		return false;*/
	return false;
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

