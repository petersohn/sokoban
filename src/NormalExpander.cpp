#include "NormalExpander.hpp"

#include "Checker/Checker.hpp"
#include "Checker/NodeChecker.hpp"
#include "Checker/VisitedStates.hpp"

#include "Dumper/Dumper.hpp"

#include "HeurCalculator/HeurCalculator.hpp"

#include "Status/Status.hpp"

#include "ExpandHelper.hpp"
#include "Node.hpp"
#include "NodeFactory.hpp"
#include "PrioNodeQueue.hpp"

#include <iostream>

class NormalExpander::InternalExpander {
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

void NormalExpander::InternalExpander::expandNode(Point p, Point d)
{
    auto expanded = createNode(status_, p, d, base_, *owner_.nodeFactory_,
            *owner_.calculator_, owner_.checker_, dumper_);
    const auto& node = expanded.first;
    const auto& status = expanded.second;

    if (!node) {
        return;
    }
    assert(status);

    if (!owner_.nodeChecker_.check(*status, *node)) {
        if (dumper_) {
            dumper_->reject(node, owner_.nodeChecker_.errorMessage());
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

void NormalExpander::InternalExpander::expand()
{
    if (dumper_ && base_) {
        dumper_->expand(base_);
    }
    expandStatus(status_, [&](Point stone, Point d) { expandNode(stone, d); });
}


NormalExpander::NormalExpander(
        std::shared_ptr<const HeurCalculator> calculator,
        ComplexChecker checker, ComplexNodeChecker nodeChecker,
        std::shared_ptr<NodeFactory> nodeFactory, std::size_t* expandedNodes):
        calculator_(std::move(calculator)),
        checker_(std::move(checker)),
        nodeChecker_(std::move(nodeChecker)),
        nodeFactory_(std::move(nodeFactory)),
        maxDepth_(0),
        expandedNodes_(expandedNodes)
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


