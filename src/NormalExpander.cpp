#include "NormalExpander.h"
#include "Status.h"
#include "Node.h"
#include <iostream>

class InternalExpander {
	const Status &status_;
	Node::Ptr base_;
	NodePusher &queue_;
	Dumper::Ptr dumper_;
	NormalExpander &owner_;
public:
	InternalExpander(const Status & status, boost::shared_ptr<Node> base,
			NodePusher & queue, Dumper::Ptr dumper, NormalExpander &owner):
				status_(status),
				base_(base),
				queue_(queue),
				dumper_(dumper),
				owner_(owner)
	{}

	void expand();
	void expandNode(const Point &p, const Point &d);
};

void InternalExpander::expandNode(const Point &p, const Point &d)
{
	Point pd = p+d, pmd = p-d;
	if (pmd.x >= 0 && pmd.x < status_.width() &&
			pmd.y >= 0 && pmd.y < status_.height() &&
			status_.value(pd) == ftFloor && status_.reachable(pmd))
	{
		Status status(status_);
		Node::Ptr node;
		status.currentPos(p);
		if (owner_.calculator_->calculateStone(status, pd) < 0 || !status.moveStone(p, pd)) {
			return;
		}
		node = Node::create(status.state(), p, d,
			base_, 1, owner_.calculator_->calculateStatus(status));
		VisitedStateInput vsi(std::make_pair<const Status&, int>(status, node->costFgv()));
		if (owner_.visitedStates_ && owner_.visitedStates_->hasElem(vsi)) {
			if (dumper_)
				dumper_->reject(node, "not movable");
			return;
		}
		if (pd != status.table().destination()) {
			if (owner_.checker_ && !owner_.checker_->check(status, pd)) {
				if (dumper_)
					dumper_->reject(node, owner_.checker_->errorMessage());
				return;
			}
		}
		queue_.push(node);
		if (dumper_)
			dumper_->addNode(node);
		if (owner_.visitedStates_)
			owner_.visitedStates_->push(vsi);
		owner_.maxDepth_ = std::max(node->depth(), owner_.maxDepth_);
		if (owner_.enableLog_ && ++owner_.expandedNodes_ % 10000 == 0)
			std::cerr << boost::format(
					"Expanded nodes: %d.\n"
					"Nodes in queue: %d.\n"
					"Maximum depth: %d.\n"
					"Cost function: %d\n") %
				owner_.expandedNodes_ % queue_.size() %
				owner_.maxDepth_ % base_->costFgv() << std::endl;
	}
}

void InternalExpander::expand()
{
	if (owner_.visitedStates_->size() == 0) {
		owner_.visitedStates_->push(std::make_pair<const Status&, int>(status_,
				owner_.calculator_->calculateStatus(status_)));
	}
	if (dumper_)
		dumper_->expand(base_);
	for (State::const_iterator it = status_.state().begin();
			it != status_.state().end(); ++it)
	{
		if (*it == status_.table().destination())
			continue;
		expandNode(*it, Point::p10);
		expandNode(*it, Point::pm10);
		expandNode(*it, Point::p01);
		expandNode(*it, Point::p0m1);
	}
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
	assert(calculator_.get() != NULL);
	assert(checker_.get() != NULL);
}

NormalExpander::~NormalExpander()
{
	if (enableLog_)
		std::cerr << "Expanded nodes: " << expandedNodes_ << std::endl;
}

bool NormalExpander::expand(const Status &status, boost::shared_ptr<Node> base,
		NodePusher& queue, Dumper::Ptr dumper) {
	InternalExpander exp(status, base, queue, dumper, *this);
	exp.expand();
	return true;
}


