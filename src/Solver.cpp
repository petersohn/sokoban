#include "Solver.h"
#include "Table.h"
#include "State.h"
#include "Status.h"
#include "Node.h"
#include "Dumper.h"
#include "StonePusher.h"
#include "XDumper.h"
#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <deque>
#include <queue>
#include <algorithm>
#include <set>
#include <memory>
#include <boost/format.hpp>
#include <boost/bind.hpp>

class InternalSolver {
	HeurCalculator::Ptr calculator_;
	FixedTable::Ptr table_;
	std::multiset<VisitedState> visitedStates_;
	std::priority_queue<Node::Ptr, std::vector<Node::Ptr>, NodeCompare> queue_;

	std::ofstream dumpFile_;
	std::auto_ptr<XDumper> xdump_;
	bool enableDump_;
	bool enableXDump_;
	bool enableLog_;
	int expandedNodes_;
	int maxDepth_;

	void expandNodes(const VisitedState &state, Node::Ptr base);
	void expandNode(const VisitedState &state, int stone,
			const Point &d, Node::Ptr base);
	bool checkStone(const Status &status, int stone);
	bool stoneMovable(const Status &status, int stone, std::set<int> &volt);
	bool isMovable(const Status &status, const Point & p,
			std::set<int> &volt, int &count);
	bool checkCorridorEnding(const Status &status,
			const Point &p0, const Point &side);
	bool checkCorridors(const Status &status, int stone);
	bool pushStones(const Status &status, Node::Ptr base);
	void addVisitedState(const VisitedState &state);
	bool statusVisited(const Status &status);
	void pushQueue(Node::Ptr node);
	Node::Ptr popQueue();
public:
	explicit InternalSolver(bool enableLog, bool enableDump, bool enableXDump);

	std::deque<Node::Ptr> solve(const Status &status, HeurCalculator::Ptr calculator);
};

InternalSolver::InternalSolver(bool enableLog, bool enableDump, bool enableXDump):
		enableLog_(enableLog),
		enableDump_(enableDump),
		enableXDump_(enableXDump)
{
	if (enableDump_) {
		static int counter = 0;
		boost::format filename("dump%1%.dump");
		if (++counter > 1)
			filename % counter;
		else
			filename % "";
		dumpFile_.open(filename.str().c_str(), std::ofstream::out | std::ofstream::trunc);
	}
}

std::deque<Node::Ptr> InternalSolver::solve(const Status &status,
		HeurCalculator::Ptr calculator)
{
	Node::Ptr nnn = Node::create();
//	_dump(status, false);
//	_dump(*nnn, false);
	calculator_ = calculator;
	table_ = status.tablePtr();
	visitedStates_.clear();
	expandedNodes_ = 0;
	maxDepth_ = 0;
	if (enableXDump_)
		xdump_.reset(new XDumper(table_));
	Node::Ptr currentNode;
	VisitedState vs(status.state());
	addVisitedState(vs);
	do
	{
		expandNodes(vs, currentNode);
		currentNode = popQueue();
		if (currentNode.get() == NULL)
			break;
		vs = VisitedState(*currentNode);
	} while (currentNode->heur() > 0);
	if (enableLog_)
		std::cerr << "Expanded nodes: " << expandedNodes_ << std::endl;
	std::deque<Node::Ptr> result = pathToRoot(currentNode);
	if (enableXDump_) {
		for (std::deque<Node::Ptr>::iterator it = result.begin();
				it != result.end(); ++it)
			xdump_->addToSolution(*it);
		xdump_->save("dump.xml");
	}
	return result;
}


void InternalSolver::expandNodes(const VisitedState &state, Node::Ptr base) {
//	if (pushStones(Status(table_, state), base))
//		return;
	if (enableDump_ && base.get() != NULL)
		dumpNode(dumpFile_, table_, *base, "Expanding");
	for (int i = 0; i < state.size(); ++i)
	{
		if (state[i] == table_->get().destination())
			continue;
		expandNode(state, i, Point::p10, base);
		expandNode(state, i, Point::pm10, base);
		expandNode(state, i, Point::p01, base);
		expandNode(state, i, Point::p0m1, base);
	}
	if (enableDump_)
		dumpFile_ << std::endl << "--------" << std::endl << std::endl;
}

void InternalSolver::expandNode(const VisitedState &state, int stone,
		const Point &d, Node::Ptr base) {
	Status status(table_, state);
	Point p = state[stone];
	Point pd = p+d, pmd = p-d;
	if (pmd.x >= 0 && pmd.x < status.width() &&
			pmd.y >= 0 && pmd.y < status.height() &&
			status.value(pd) == ftFloor && status.reachable(pmd))
	{
		Node::Ptr node;
		status.currentPos(p);
		if (calculator_->calculateStone(status, pd) < 0 || !status.moveStone(stone, pd)) {
			return;
		}
		if (enableXDump_) {
			node = Node::create(status.state(), stone, d,
				base, 1, calculator_->calculateStatus(status));
			xdump_->addNode(node);
		}
		if (statusVisited(status)) {
			if (enableXDump_)
				xdump_->reject(node, "already visited");
			return;
		}
		if (status.state()[stone] != table_->get().destination()) {
			std::set<int> volt;
			if (!stoneMovable(status, stone, volt)) {
				if (enableXDump_)
					xdump_->reject(node, "not movable");
				return;
			}
			if (!checkCorridors(status, stone)) {
				if (enableXDump_)
					xdump_->reject(node, "corridor found");
				return;
			}
		}
		if (!enableXDump_)
			node = Node::create(status.state(), stone, d,
				base, 1, calculator_->calculateStatus(status));
		pushQueue(node);
		addVisitedState(status.state());
		if (enableDump_)
			dumpNode(dumpFile_, table_, *node, "Added");
		maxDepth_ = std::max(node->depth(), maxDepth_);
		if (enableLog_ && ++expandedNodes_ % 10000 == 0)
			std::cerr << boost::format("%d (%d, %d [%d])") %
				expandedNodes_ % queue_.size() %
				node->depth() % maxDepth_ << std::endl;
	}
}

bool InternalSolver::checkStone(const Status &status, int stone) {
	assert(status.tablePtr() == table_);
	std::set<int> volt;
	bool result = stoneMovable(status, stone, volt) && checkCorridors(status, stone);
	return result;
}

bool InternalSolver::stoneMovable(const Status &status, int stone, std::set<int> &volt)
{
	assert(status.tablePtr() == table_);
	Point p = status.state()[stone];
	volt.insert(stone);
	int count = 0;
	if (isMovable(status, p+Point::p10, volt, count) &&
			isMovable(status, p+Point::pm10, volt, count) && count > 0)
		return true;
	count = 0;
	return (isMovable(status, p+Point::p01, volt, count) &&
			isMovable(status, p+Point::p0m1, volt, count) && count > 0);
}

bool InternalSolver::isMovable(const Status &status, const Point & p,
		std::set<int> &volt, int &count)
{
	assert(status.tablePtr() == table_);
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
	return stoneMovable(status, status.stoneAt(p), volt);
}

bool InternalSolver::checkCorridors(const Status &status, int stone) {
	assert(status.tablePtr() == table_);
	Point p0 = status.state()[stone];
	bool kell[3][3];
	for (int x = 0; x < 3; x++)
		for (int y = 0; y < 3; y++)
			kell[x][y] = true;

	for (int x = 0; x < 3; x++)
		for (int y = 0; y < 3; y++) {
			Point p = p0 + Point(x - 1, y - 1);
			if (!kell[x][y] || status.value(p) != ftFloor || status.reachable(p))
				continue;
			Array<bool> reach(status.width(), status.height(), false);
			MinMax minmax;
			floodFill(status, p, reach, NULL, &minmax);
			if (!reach[status.table().destination()]) {
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
						if (enableDump_) {
							dumpStatus(dumpFile_, status, "1x1 corridor found", &reach);
						}
						return false;
					}
				} else
				if (minmax.minX == minmax.maxX) {
					if (!checkCorridorEnding(status,
								Point(minmax.minX, minmax.minY - 1), Point::p10) &&
						!checkCorridorEnding(status,
								Point(minmax.minX, minmax.maxY + 1), Point::p10)) {
						if (enableDump_) {
							dumpStatus(dumpFile_, status, "Vertical corridor found", &reach);
						}
						return false;
					}
				} else
				if (minmax.minY == minmax.maxY) {
					if (!checkCorridorEnding(status,
								Point(minmax.minX - 1, minmax.minY), Point::p01) &&
						!checkCorridorEnding(status,
								Point(minmax.maxX + 1, minmax.minY), Point::p01)) {
						if (enableDump_)
							dumpStatus(dumpFile_, status, "Horizontal corridor found", &reach);
						return false;
					}
				}
			}
			for (int xx = 0; xx < 3; xx++)
				for (int yy = 0; yy < 3; yy++) {
					Point pp = p0 + Point(xx - 1, yy - 1);
					if (arrayAt<bool>(reach, pp, false))
						kell[xx][yy] = false;
				}
		}
	return true;
}

bool InternalSolver::checkCorridorEnding(const Status &status,
		const Point &p0, const Point &side) {
	assert(status.tablePtr() == table_);
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
			(calculator_->calculateStone(status, p1) || calculator_->calculateStone(status, pm1));
}

bool InternalSolver::pushStones(const Status &status, Node::Ptr base)
{
//	StonePusher sp(calculator_);
//	Node::Ptr node = sp.pushStones(status, base);
//	if (node.get() == NULL)
//		return false;
//	pushQueue(node);
//	std::for_each(sp.pushList().begin(), sp.pushList().end(),
//			boost::bind(&InternalSolver::addVisitedState, this, _1));
//	return true;
}

void InternalSolver::addVisitedState(const VisitedState &state) {
	visitedStates_.insert(state);
}

bool InternalSolver::statusVisited(const Status &status) {
	assert(status.tablePtr() == table_);
	std::pair<std::multiset<VisitedState>::iterator,
		std::multiset<VisitedState>::iterator> found =
			visitedStates_.equal_range(status.state());
	if (found.first != found.second)
	{
		Array<bool> reach(status.width(), status.height(), false);
		floodFill(status, status.state().currentPos(), reach);
		for (std::multiset<VisitedState>::iterator it = found.first;
				it != found.second; ++it) {
			if (reach[it->currentPos()] && *it == status.state()) {
				if (enableDump_)
					dumpStatus(dumpFile_, status, "Already visited");
				return true;
			}
		}
	}
	return false;
}

void InternalSolver::pushQueue(Node::Ptr node) {
	queue_.push(node);
}

Node::Ptr InternalSolver::popQueue() {
	if (queue_.empty())
		return Node::Ptr();
	Node::Ptr result = queue_.top();
	queue_.pop();
	return result;
}




std::deque<Node::Ptr> Solver::solve(const Status &status,
		HeurCalculator::Ptr calculator, bool enableLog, bool enableDump,
		bool enableXDump)
{
	InternalSolver solver(enableLog, enableDump, enableXDump);
	return solver.solve(status, calculator);
}

