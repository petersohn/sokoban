#ifndef NODEFACTORY_HPP_
#define NODEFACTORY_HPP_

#include "Node.hpp"
#include "HeurCalculator.hpp"
#include "Status/Status.hpp"
#include <memory>

class NodeFactory {
	int numNodes_;
	std::shared_ptr<const HeurCalculator> calculator_;
	std::shared_ptr<const HeurCalculator> experimentalCalculator_;
public:

	NodeFactory(
			const std::shared_ptr<const HeurCalculator>& calculator,
			const std::shared_ptr<const HeurCalculator>& experimentalCalculator = std::shared_ptr<const HeurCalculator>()):
		numNodes_(0),
		calculator_(calculator),
		experimentalCalculator_(experimentalCalculator)
	{}
	std::shared_ptr<Node> createNode(
			const Status & status,
			const MoveDescriptor& moveDescriptor,
			const std::shared_ptr<Node>& ancestor)
	{
		return std::make_shared<Node>(
				status.state(), moveDescriptor, ancestor, 1,
				calculator_->calculateStatus(status, &moveDescriptor, ancestor),
				experimentalCalculator_ ?
						experimentalCalculator_->calculateStatus(status, &moveDescriptor, ancestor) :
						0,
				++numNodes_);
	}
};


#endif /* NODEFACTORY_HPP_ */
