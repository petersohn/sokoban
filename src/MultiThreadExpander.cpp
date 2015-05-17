#include "MultiThreadExpander.hpp"

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

class MultiThreadExpander::InternalExpander {
    const Status& status_;
    std::shared_ptr<Node> base_;
    PrioNodeQueue& queue_;
    Dumper* dumper_;
    MultiThreadExpander& owner_;

    std::vector<std::vector<std::pair<std::shared_ptr<Node>, Status>>> nodesToExpand;
public:
    InternalExpander(const Status& status, std::shared_ptr<Node> base,
            PrioNodeQueue& queue, Dumper* dumper, MultiThreadExpander& owner):
                status_(status),
                base_(std::move(base)),
                queue_(queue),
                dumper_(dumper),
                owner_(owner),
                nodesToExpand(owner.threadPool_.getNumThreads())
    {
    }

    void expand();
    void prepareNode(Point p, Point d);
    void expandNode(const std::shared_ptr<Node>& node, const Status& status);
};

void MultiThreadExpander::InternalExpander::prepareNode(Point p, Point d)
{
    auto expanded = createNode(status_, p, d, base_, *owner_.nodeFactory_,
            *owner_.calculator_, owner_.checker_, nullptr);
    const auto& node = expanded.first;
    const auto& status = expanded.second;

    if (node) {
        assert(status);
        nodesToExpand[*util::ThreadPool::getCurrentThreadId()].emplace_back(
                node, *status);
    }

}

void MultiThreadExpander::InternalExpander::expandNode(
        const std::shared_ptr<Node>& node, const Status& status)
{
    if (!owner_.nodeChecker_.check(status, *node)) {
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

void MultiThreadExpander::InternalExpander::expand()
    {
        if (dumper_ && base_) {
            dumper_->expand(base_);
    }

    owner_.worker_.startFilling();
    expandStatus(status_, [&](Point stone, Point d) {
            owner_.worker_.addAction([this, stone, d]() {
                    prepareNode(stone, d);
                });
        });
    owner_.worker_.finishFilling();
    owner_.worker_.wait(false);

    for (const auto& nodes: nodesToExpand) {
        for (const auto& value: nodes) {
            expandNode(value.first, value.second);
        }
    }
}



MultiThreadExpander::MultiThreadExpander(
        std::shared_ptr<const HeurCalculator> calculator,
        ComplexChecker checker, ComplexNodeChecker nodeChecker,
        std::shared_ptr<NodeFactory> nodeFactory, std::size_t* expandedNodes,
        std::size_t numThreads):
        calculator_(std::move(calculator)),
        checker_(std::move(checker)),
        nodeChecker_(std::move(nodeChecker)),
        nodeFactory_(std::move(nodeFactory)),
        worker_(threadPool_.getIoService(), 1),
        maxDepth_(0),
        expandedNodes_(expandedNodes)
{
    assert(calculator_.get() != NULL);
    threadPool_.setNumThreads(numThreads);
    threadPool_.start();
}

MultiThreadExpander::~MultiThreadExpander()
{
    threadPool_.wait();
}

void MultiThreadExpander::expand(const Status& status, std::shared_ptr<Node> base,
        PrioNodeQueue& queue, std::shared_ptr<Dumper> dumper) {
    status.reachableArray();
    InternalExpander exp(status, std::move(base), queue, dumper.get(), *this);
    exp.expand();
}



