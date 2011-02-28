#include "Solver.h"
#include "Table.h"
#include "State.h"
#include "Status.h"
#include "Node.h"
#include "Dumper.h"
#include "StonePusher.h"
#include "VisitedStateInfo.h"
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
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>

class InternalSolver {
	HeurCalculator::Ptr calculator_;
	FixedTable::Ptr table_;
	typedef boost::unordered_multimap<State, VisitedStateInfo, boost::hash<State> > VisitedStateSet;
	VisitedStateSet visitedStates_;
	std::priority_queue<Node::Ptr, std::vector<Node::Ptr>, NodeCompare> queue_;

	std::ofstream dumpFile_;
	std::auto_ptr<XDumper> xdump_;
	bool enableDump_;
	bool enableXDump_;
	bool enableLog_;
	int expandedNodes_;
	int maxDepth_;

	void expandNodes(const Status &status, Node::Ptr base);
	void expandNode(const Status &st, const Point &p,
			const Point &d, Node::Ptr base);
	bool checkStone(const Status &status, int stone);
	bool stoneMovable(const Status &status, const Point &p,
			boost::unordered_set<Point> &volt);
	bool isMovable(const Status &status, const Point & p,
			boost::unordered_set<Point> &volt, int &count);
	bool checkCorridorEnding(const Status &status,
			const Point &p0, const Point &side);
	bool checkCorridors(const Status &status, const Point &p0);
	bool pushStones(const Status &status, Node::Ptr base);
	void addVisitedState(const Status &st, int heur);
	bool statusVisited(const Status &status, int heur);
	void pushQueue(Node::Ptr node);
	Node::Ptr popQueue();
public:
	explicit InternalSolver(bool enableLog, bool enableDump, bool enableXDump);

	std::deque<Node::Ptr> solve(Status status, HeurCalculator::Ptr calculator);
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

std::deque<Node::Ptr> InternalSolver::solve(Status status,
		HeurCalculator::Ptr calculator)
{
	Node::Ptr nnn = Node::create();
	calculator_ = calculator;
	table_ = status.tablePtr();
	visitedStates_.clear();
	expandedNodes_ = 0;
	maxDepth_ = 0;
	if (enableXDump_)
		xdump_.reset(new XDumper(table_));
	Node::Ptr currentNode;
	addVisitedState(status, calculator_->calculateStatus(status));
	do
	{
		expandNodes(status, currentNode);
		currentNode = popQueue();
		if (currentNode.get() == NULL)
			break;
		status.set(*currentNode);
	} while (currentNode->heur() > 0);
	if (enableLog_)
		std::cerr << "Expanded nodes: " << expandedNodes_ << std::endl;
	std::deque<Node::Ptr> result = pathToRoot(currentNode);
	if (enableXDump_) {
		for (std::deque<Node::Ptr>::iterator it = result.begin();
				it != result.end(); ++it)
			xdump_->addToSolution(*it);
		xdump_->save("dump.xml");
//		xdump_->dump();
	}
	return result;
}


void InternalSolver::expandNodes(const Status &status, Node::Ptr base) {
	if (pushStones(status, base))
		return;
	if (enableXDump_ && base.get() != NULL) {
		xdump_->expand(base);
	}
	if (enableDump_ && base.get() != NULL)
		dumpNode(dumpFile_, table_, *base, "Expanding");
	for (State::const_iterator it = status.state().begin();
			it != status.state().end(); ++it)
	{
		if (*it == table_->get().destination())
			continue;
		expandNode(status, *it, Point::p10, base);
		expandNode(status, *it, Point::pm10, base);
		expandNode(status, *it, Point::p01, base);
		expandNode(status, *it, Point::p0m1, base);
	}
	if (enableDump_)
		dumpFile_ << std::endl << "--------" << std::endl << std::endl;
}

void InternalSolver::expandNode(const Status &st, const Point &p,
		const Point &d, Node::Ptr base) {
	Point pd = p+d, pmd = p-d;
	if (pmd.x >= 0 && pmd.x < st.width() &&
			pmd.y >= 0 && pmd.y < st.height() &&
			st.value(pd) == ftFloor && st.reachable(pmd))
	{
		Status status(st);
		Node::Ptr node;
		status.currentPos(p);
		if (calculator_->calculateStone(status, pd) < 0 || !status.moveStone(p, pd)) {
			return;
		}
		node = Node::create(status.state(), p, d,
			base, 1, calculator_->calculateStatus(status));
		if (enableXDump_) {
			xdump_->addNode(node);
		}
		if (statusVisited(status, node->costFgv())) {
			if (enableXDump_)
				xdump_->reject(node, "already visited");
			return;
		}
		if (pd != table_->get().destination()) {
			boost::unordered_set<Point> volt;
			if (!stoneMovable(status, pd, volt)) {
				if (enableXDump_)
					xdump_->reject(node, "not movable");
				return;
			}
			if (!checkCorridors(status, pd)) {
				if (enableXDump_)
					xdump_->reject(node, "corridor found");
				return;
			}
		}
		pushQueue(node);
		addVisitedState(status, node->costFgv());
		if (enableDump_)
			dumpNode(dumpFile_, table_, *node, "Added");
		maxDepth_ = std::max(node->depth(), maxDepth_);
		if (enableLog_ && ++expandedNodes_ % 10000 == 0)
			std::cerr << boost::format(
					"Expanded nodes: %d.\n"
					"Nodes in queue: %d.\n"
					"Maximum depth: %d.\n"
					"Cost function: %d\n") %
				expandedNodes_ % queue_.size() %
				maxDepth_ % base->costFgv() << std::endl;
	}
}

bool InternalSolver::checkStone(const Status &status, int stone) {
	assert(false);
//	assert(status.tablePtr() == table_);
//	std::set<int> volt;
//	bool result = stoneMovable(status, stone, volt) && checkCorridors(status, stone);
//	return result;
}

bool InternalSolver::stoneMovable(const Status &status, const Point &p,
		boost::unordered_set<Point> &volt)
{
	assert(status.tablePtr() == table_);
	volt.insert(p);
	int count = 0;
	if (isMovable(status, p+Point::p10, volt, count) &&
			isMovable(status, p+Point::pm10, volt, count) && count > 0)
		return true;
	count = 0;
	return (isMovable(status, p+Point::p01, volt, count) &&
			isMovable(status, p+Point::p0m1, volt, count) && count > 0);
}

bool InternalSolver::isMovable(const Status &status, const Point & p,
		boost::unordered_set<Point> &volt, int &count)
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
	if (volt.count(p) != 0)
		return false;
	return stoneMovable(status, p, volt);
}

bool InternalSolver::checkCorridors(const Status &status, const Point &p0) {
	assert(status.tablePtr() == table_);
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
	StonePusher sp(calculator_);
//	dumpStatus(std::cerr, status, "push");
	Node::Ptr node = sp.pushStones(status, base);
	if (node.get() == NULL)
		return false;
	pushQueue(node);
	Status st(table_);
	std::deque<Node::Ptr> path = pathToBase(node, base);
	for (std::deque<Node::Ptr>::iterator it = path.begin();
			it != path.end(); ++it) {
		st.set(**it);
		addVisitedState(st, (*it)->costFgv());
		if (enableXDump_) {
			xdump_->addNode(*it);
			if (*it != node)
				xdump_->push(*it);
		}
	}
	if (enableXDump_)
		xdump_->push(base);
	return true;
}

void InternalSolver::addVisitedState(const Status &st, int heur) {
	assert(st.tablePtr() == table_);
	visitedStates_.insert(std::make_pair(st.state(), VisitedStateInfo(st.currentPos(), heur)));
}

bool InternalSolver::statusVisited(const Status &status, int heur) {
	assert(status.tablePtr() == table_);
	std::pair<VisitedStateSet::iterator,
		VisitedStateSet::iterator> found =
			visitedStates_.equal_range(status.state());
	if (found.first != found.second)
	{
		for (VisitedStateSet::iterator it = found.first;
				it != found.second; ++it) {
			if (status.reachable(it->second.currentPos())) {
				if (heur < it->second.heur()) {
					if (enableDump_)
						dumpStatus(dumpFile_, status,
								(boost::format("Already visited, with worse heur (%d --> %d)") %
								it->second.heur() % heur).str());
				} else {
					if (enableDump_)
						dumpStatus(dumpFile_, status, "Already visited");
					return true;
				}
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

