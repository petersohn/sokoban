#ifndef SRC_EXPANDHELPER_HPP
#define SRC_EXPANDHELPER_HPP

#include "Status/Point.hpp"
#include "Status/Status.hpp"
#include "Node.hpp"
#include "NodeFactory.hpp"
#include "Dumper/Dumper.hpp"
#include <memory>
#include <boost/optional.hpp>

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

	std::shared_ptr<Node> node =
			nodeFactory.createNode(status, MoveDescriptor(p, d), base);
	if (pd != status.table().destination()) {
		if (!checker.check(status, pd)) {
			if (dumper)
				dumper->reject(node, checker.errorMessage());
			return {};
		}
	}

	return {node, status};
}

#endif /* SRC_EXPANDHELPER_HPP */
