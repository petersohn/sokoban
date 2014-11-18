#include "NormalExpander.hpp"
#include "Status/Status.hpp"
#include "Node.hpp"
#include "FieldType.hpp"
#include "VisitedStates.hpp"
#include "PrioNodeQueue.hpp"
#include <iostream>
#include <boost/thread/locks.hpp>

class InternalExpander {
	const Status &status_;
	Node::Ptr base_;
	PrioNodeQueue& queue_;
	Dumper* dumper_;
	NormalExpander& owner_;
public:
	InternalExpander(const Status& status, std::shared_ptr<Node> base,
			PrioNodeQueue& queue, Dumper* dumper, NormalExpander &owner):
				status_(status),
				base_(std::move(base)),
				queue_(queue),
				dumper_(dumper),
				owner_(owner)
	{
	}

	void expand();
	void expandNode(Point p, Point d);
};

void InternalExpander::expandNode(Point p, Point d)
{
	Point pd = p+d, pmd = p-d;
	if (pmd.x >= 0 && pmd.x < static_cast<int>(status_.width()) &&
			pmd.y >= 0 && pmd.y < static_cast<int>(status_.height()) &&
			status_.value(pd) == FieldType::floor && status_.reachable(pmd))
	{
		Status status(status_);
		status.currentPos(p);
		if (owner_.calculator_->calculateStone(status, pd) < 0 || !status.moveStone(p, pd)) {
			return;
		}
		Node::Ptr node =
				owner_.nodeFactory_->createNode(status, MoveDescriptor(p, d), base_);
		if (pd != status.table().destination()) {
			if (!owner_.checker_.check(status, pd)) {
				if (dumper_)
					dumper_->reject(node, owner_.checker_.errorMessage());
				return;
			}
		}
		VisitedStateInput vsi(status, node->costFgv());
		if (!owner_.visitedStates_->checkAndPush(vsi)) {
			if (dumper_)
				dumper_->reject(node, "already visited");
			return;
		}
		queue_.push(node);
		if (dumper_)
			dumper_->addNode(node);
		{
			owner_.maxDepth_ = std::max(node->depth(), owner_.maxDepth_);
			if (owner_.enableLog_ && ++owner_.expandedNodes_ % 10000 == 0)
				std::cerr << boost::format(
						"Expanded nodes: %d.\n"
						"Nodes in queue: %d.\n"
						"Maximum depth: %d.\n"
						"Cost function: %d\n") %
					owner_.expandedNodes_ % queue_.size() %
					owner_.maxDepth_ % (base_ ? base_->costFgv() : -1) << std::endl;
		}
	}
}

void InternalExpander::expand()
{
	if (owner_.visitedStates_->empty()) {
		owner_.visitedStates_->checkAndPush(std::pair<const Status&, int>(status_,
				owner_.calculator_->calculateStatus(status_)));
	}
	if (dumper_ && base_)
		dumper_->expand(base_);
	for (const auto& state: status_.state())
	{
		if (state == status_.table().destination())
			continue;
		expandNode(state, Point::p10);
		expandNode(state, Point::pm10);
		expandNode(state, Point::p01);
		expandNode(state, Point::p0m1);
	}
}



NormalExpander::NormalExpander(
		std::shared_ptr<VisitedStates> vs, HeurCalculator::Ptr calculator,
		ComplexChecker ch, NodeFactory::Ptr nodeFactory, bool enableLog):
		visitedStates_(std::move(vs)),
		calculator_(std::move(calculator)),
		checker_(std::move(ch)),
		nodeFactory_(std::move(nodeFactory)),
		maxDepth_(0),
		enableLog_(enableLog),
		expandedNodes_(0)
{
	assert(calculator_.get() != NULL);
}

NormalExpander::~NormalExpander()
{
	if (enableLog_)
		std::cerr << "Expanded nodes: " << expandedNodes_ << std::endl;
}

void NormalExpander::expand(const Status& status, std::shared_ptr<Node> base,
		PrioNodeQueue& queue, Dumper::Ptr dumper) {
	InternalExpander exp(status, std::move(base), queue, dumper.get(), *this);
	exp.expand();
}


