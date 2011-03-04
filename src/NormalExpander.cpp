#include "NormalExpander.h"
#include "Status.h"
#include "Node.h"

void NormalExpander::expandNode(const Status & st, boost::shared_ptr<Node> base, NodePusher & queue)
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
		if (visitedStates_ && visitedStates_->hasElem(status, node->costFgv())) {
			return;
		}
		if (pd != table_->get().destination()) {
			if (checker_ && !checker_->check(status))
				return;
		}
		queue.push(node);
		if (visitedStates_)
			visitedStates_->push(status, node->costFgv());
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

bool NormalExpander::expand(const Status & status, boost::shared_ptr<Node> base, NodePusher & queue)
{
	assert(queue.get() != NULL);
	for (State::const_iterator it = status.state().begin();
			it != status.state().end(); ++it)
	{
		if (*it == status.table().destination())
			continue;
		expandNode(status, *it, Point::p10, base);
		expandNode(status, *it, Point::pm10, base);
		expandNode(status, *it, Point::p01, base);
		expandNode(status, *it, Point::p0m1, base);
	}
	return true;
}

NormalExpander::NormalExpander(VisitedStateHolder::Ptr vs, HeurCalculator::Ptr calculator,
		Checker::Ptr ch, bool enableLog):
		visitedStates_(vs),
		calculator_(calculator),
		checker_(ch),
		maxDepth(0),
		enableLog_(enableLog),
		expandedNodes(0)
{
	assert(calculator != NULL);
}

