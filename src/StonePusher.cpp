#include "StonePusher.hpp"

#include "Checker/NodeChecker.hpp"
#include "Checker/VisitedStates.hpp"

#include "Dumper/Dumper.hpp"

#include "HeurCalculator/HeurCalculator.hpp"

#include "Status/floodFill.hpp"
#include "Status/Status.hpp"

#include "Matrix.hpp"
#include "FieldType.hpp"
#include "intersect.hpp"
#include "Node.hpp"
#include "NodeFactory.hpp"
#include "PrioNodeQueue.hpp"

#include <iostream>
#include <memory>

namespace sokoban {

class InternalPusher {
public:
private:
    std::shared_ptr<Node> node_;
    const HeurCalculator& calculator_;
    NodeFactory& nodeFactory_;
    bool pushStone(const Status& status, Point p);
    bool pushStoneIter(const Status& status, Point p, Point d);
public:
    InternalPusher(const HeurCalculator& calculator, NodeFactory& nodeFactory):
        calculator_(calculator),
        nodeFactory_(nodeFactory)
    {}
    std::shared_ptr<Node> pushStones(Status status, std::shared_ptr<Node> base);
};

std::shared_ptr<Node> InternalPusher::pushStones(Status status, std::shared_ptr<Node> base) {
    node_ = std::move(base);
    Matrix<bool> destReachable(status.width(), status.height(), false);
    bool touched;
    bool touched2 = false;

    do {
        touched = false;
        Status::BorderType destBorder;
        floodFill(status, status.table().destination(), destReachable, destBorder);
        Status::BorderType border = intersect(status.border(),
                std::move(destBorder));
        for (Status::BorderType::const_iterator it = border.begin();
                it != border.end(); ++it) {
            if (pushStone(status, *it)) {
                status.set(*node_);
                touched = true;
            }
        }
        if (touched)
            touched2 = true;
    } while (touched);
    return touched2 ? node_ : std::shared_ptr<Node>();
}

bool InternalPusher::pushStone(const Status& status, Point p) {
    if (p == status.table().destination())
        return true;
    if (pushStoneIter(status, p, p10))
        return true;
    if (pushStoneIter(status, p, -p10))
        return true;
    if (pushStoneIter(status, p, p01))
        return true;
    if (pushStoneIter(status, p, -p01))
        return true;
    return false;
}

bool InternalPusher::pushStoneIter(const Status& status, Point p, Point d) {
    Point pd = p+d;
    Point pmd = p-d;
    if (status.value(pmd) != FieldType::floor || status.value(pd) != FieldType::floor || !status.reachable(pmd))
        return false;
    Status newStatus(status);
    if (newStatus.currentPos() == pd) {
        newStatus.shiftCurrentPos();
    }

    float heur1 = calculator_.calculateStone(newStatus, p);
    newStatus.currentPos(p);
    float heur2 = calculator_.calculateStone(newStatus, pd);
    if ((heur2 < 0 && (pd != newStatus.table().destination())) || heur2 >= heur1) {
        return false;
    }

    if (!newStatus.moveStone(p, pd)) {
        std::cerr << "Whoopsie doopsie!";
        return false; // should never happen
    }

    node_ = nodeFactory_.createNode(newStatus, MoveDescriptor(p, d), node_);

    if (pushStone(newStatus, pd)) {
        return true;
    }

    node_ = node_->ancestor();
    return false;
}


StonePusher::StonePusher(std::unique_ptr<Expander> expander,
        ComplexNodeChecker nodeChecker,
        std::shared_ptr<const HeurCalculator> calculator,
        std::shared_ptr<NodeFactory> nodeFactory):
        expander_(std::move(expander)),
        nodeChecker_(std::move(nodeChecker)),
        calculator_(std::move(calculator)),
        nodeFactory_(std::move(nodeFactory))

{
    assert(calculator_ != nullptr);
}

void StonePusher::expand(const Status& status, std::shared_ptr<Node> base,
        PrioNodeQueue& queue, Dumper* dumper)
{
    InternalPusher sp(*calculator_, *nodeFactory_);
    std::shared_ptr<Node> node = sp.pushStones(status, base);

    if (!node) {
        expander_->expand(status, std::move(base), queue, dumper);
        return;
    }

    Status newStatus(status.table(), *node);

    if (!nodeChecker_.check(newStatus, std::ref(*node))) {
        if (dumper) {
            dumper->reject(node, nodeChecker_.errorMessage());
        }
        return;
    }

    if (dumper) {
        if (base) {
            dumper->startPushing(base);
        }

        std::deque<std::shared_ptr<Node>> path = pathToBase(node, base);
        for (const auto& node: path) {
            dumper->push(node);
        }
    }

    if (node->heur() == 0) {
        queue.push(node);
        return;
    }

    expander_->expand(newStatus, std::move(node), queue, dumper);
}

} // namespace sokoban
