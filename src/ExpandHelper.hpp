#ifndef SRC_EXPANDHELPER_HPP
#define SRC_EXPANDHELPER_HPP

#include "Dumper/Dumper.hpp"
#include "Status/Point.hpp"
#include "Status/Status.hpp"
#include "Node.hpp"
#include "NodeFactory.hpp"

#include <boost/optional.hpp>

#include <memory>

namespace sokoban {

template <typename Function>
void expandStatus(const Status& status, Function expandFunction)
{
    for (const auto& stone: status.state())
    {
        if (stone == status.table().destination())
            continue;
        expandFunction(stone, p10);
        expandFunction(stone, -p10);
        expandFunction(stone, p01);
        expandFunction(stone, -p01);
    }
}

template <typename NodeFactory, typename HeurCalculator, typename Checker>
std::pair<std::shared_ptr<Node>, boost::optional<Status>>
createNode(const Status& originalStatus, Point p, Point d,
        const std::shared_ptr<Node>& base, NodeFactory& nodeFactory,
        const HeurCalculator& heurCalculator, const Checker& checker,
        Dumper* dumper)
{
    Point pd = p+d, pmd = p-d;
    if (originalStatus.value(pd) != FieldType::floor ||
            !originalStatus.reachable(pmd)) {
        return {};
    }

    Status status(originalStatus);
    status.currentPos(p);
    if (heurCalculator.calculateStone(status, pd) < 0 ||
            !status.moveStone(p, pd)) {
        return {};
    }

    auto doCreateNode =
            [&]() {
                return nodeFactory.createNode(
                        status, MoveDescriptor(p, d), base);
            };
    if (pd != status.table().destination()) {
        if (!checker.check(status, pd)) {
            if (dumper) {
                dumper->reject(doCreateNode(), checker.errorMessage());
            }
            return {};
        }
    }

    return {doCreateNode(), std::move(status)};
}

} // namespace sokoban

#endif /* SRC_EXPANDHELPER_HPP */
