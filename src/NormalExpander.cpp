#include "NormalExpander.hpp"
#include "Status/Status.hpp"
#include "Node.hpp"
#include "FieldType.hpp"
#include "VisitedStates.hpp"
#include "PrioNodeQueue.hpp"
#include "HeurCalculator.hpp"
#include "Dumper/Dumper.hpp"
#include "NodeFactory.hpp"
#include <iostream>
#include <boost/thread/locks.hpp>

class InternalExpander {
	const Status& status_;
	std::shared_ptr<Node> base_;
	PrioNodeQueue& queue_;
	Dumper* dumper_;
	NormalExpander& owner_;
public:
	InternalExpander(const Status& status, std::shared_ptr<Node> base,
			PrioNodeQueue& queue, Dumper* dumper, NormalExpander& owner):
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
	if (status_.value(pd) == FieldType::floor && status_.reachable(pmd)) {
		Status status(status_);
		status.currentPos(p);
		if (owner_.calculator_->calculateStone(status, pd) < 0 ||
				!status.moveStone(p, pd)) {
			return;
		}
		std::shared_ptr<Node> node =
				owner_.nodeFactory_->createNode(status, MoveDescriptor(p, d), base_);
		if (owner_.expandedNodes_ && owner_.expandedNodeLimit_ > 0 &&
				*owner_.expandedNodes_ >= owner_.expandedNodeLimit_ &&
				node->heur() > 0) {
			if (dumper_) {
				dumper_->reject(node, "node limit exceeded");
			}
			return;
		}
		if (pd != status.table().destination()) {
			if (!owner_.checker_.check(status, pd)) {
				if (dumper_)
					dumper_->reject(node, owner_.checker_.errorMessage());
				return;
			}
		}
		VisitedStateInput vsi(status, node->costFgv());
		if (!owner_.visitedStates_->checkAndPush(vsi)) {
			if (dumper_) {
				dumper_->reject(node, "already visited");
			}
			return;
		}
		queue_.push(node);
		if (dumper_) {
			dumper_->addNode(node);
		}
		owner_.maxDepth_ = std::max(node->depth(), owner_.maxDepth_);
		if (owner_.expandedNodes_ && ++*owner_.expandedNodes_ % 10000 == 0) {
			std::cerr << boost::format(
					"Expanded nodes: %d.\n"
					"Nodes in queue: %d.\n"
					"Maximum depth: %d.\n"
					"Cost function: %d\n") %
				*owner_.expandedNodes_ % queue_.size() %
				owner_.maxDepth_ % (base_ ? base_->costFgv() : -1) <<
				std::endl;
		}
	}
}

void InternalExpander::expand()
{
	if (owner_.visitedStates_->empty()) {
		owner_.visitedStates_->checkAndPush(std::pair<const Status&, int>(status_,
				owner_.calculator_->calculateStatus(status_)));
	}
	if (dumper_ && base_) {
		dumper_->expand(base_);
	}
	for (const auto& state: status_.state())
	{
		if (state == status_.table().destination())
			continue;
		expandNode(state, p10);
		expandNode(state, -p10);
		expandNode(state, p01);
		expandNode(state, -p01);
	}
}



NormalExpander::NormalExpander(
		std::shared_ptr<VisitedStates> vs, std::shared_ptr<const HeurCalculator> calculator,
		ComplexChecker ch, std::shared_ptr<NodeFactory> nodeFactory,
		std::size_t* expandedNodes, std::size_t expandedNodeLimit):
		visitedStates_(std::move(vs)),
		calculator_(std::move(calculator)),
		checker_(std::move(ch)),
		nodeFactory_(std::move(nodeFactory)),
		maxDepth_(0),
		expandedNodes_(expandedNodes),
		expandedNodeLimit_(expandedNodeLimit)
{
	assert(calculator_.get() != NULL);
}

NormalExpander::~NormalExpander()
{
}

void NormalExpander::expand(const Status& status, std::shared_ptr<Node> base,
		PrioNodeQueue& queue, std::shared_ptr<Dumper> dumper) {
	InternalExpander exp(status, std::move(base), queue, dumper.get(), *this);
	exp.expand();
}


