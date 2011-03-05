#include "NormalExpander.h"
#include "Status.h"
#include "Node.h"
#include <iostream>

void NormalExpander::expandNode(const Status &st, const Point &p, const Point &d,
			boost::shared_ptr<Node> base, NodePusher& queue)
{
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
		VisitedStateInput vsi(std::make_pair(status, node->costFgv()));
		if (visitedStates_ && visitedStates_->hasElem(vsi)) {
			return;
		}
		if (pd != status.table().destination()) {
			if (checker_ && !checker_->check(status, pd))
				return;
		}
		queue.push(node);
		if (visitedStates_)
			visitedStates_->push(vsi);
		maxDepth_ = std::max(node->depth(), maxDepth_);
		if (enableLog_ && ++expandedNodes_ % 10000 == 0)
			std::cerr << boost::format(
					"Expanded nodes: %d.\n"
					"Nodes in queue: %d.\n"
					"Maximum depth: %d.\n"
					"Cost function: %d\n") %
				expandedNodes_ % queue.size() %
				maxDepth_ % base->costFgv() << std::endl;
	}
}

bool NormalExpander::expand(const Status & status, boost::shared_ptr<Node> base, NodePusher & queue)
{
	if (visitedStates_->size() == 0) {
		visitedStates_->push(std::make_pair(status, calculator_->calculateStatus(status)));
	}
	for (State::const_iterator it = status.state().begin();
			it != status.state().end(); ++it)
	{
		if (*it == status.table().destination())
			continue;
		expandNode(status, *it, Point::p10, base, queue);
		expandNode(status, *it, Point::pm10, base, queue);
		expandNode(status, *it, Point::p01, base, queue);
		expandNode(status, *it, Point::p0m1, base, queue);
	}
	return true;
}

NormalExpander::NormalExpander(VisitedStateHolder::Ptr vs, HeurCalculator::Ptr calculator,
		Checker::Ptr ch, bool enableLog):
		visitedStates_(vs),
		calculator_(calculator),
		checker_(ch),
		maxDepth_(0),
		enableLog_(enableLog),
		expandedNodes_(0)
{
	assert(calculator_ != NULL);
	assert(checker_ != NULL);
}

NormalExpander::~NormalExpander()
{
	if (enableLog_)
		std::cerr << "Expanded nodes: " << expandedNodes_ << std::endl;
}

