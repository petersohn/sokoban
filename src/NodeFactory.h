#ifndef NODEFACTORY_HPP_
#define NODEFACTORY_HPP_

#include "Node.h"
#include "HeurCalculator.h"
#include "Status/Status.h"
#include <memory>

class NodeFactory {
	int numNodes_;
	HeurCalculator::Ptr calculator_;
	HeurCalculator::Ptr experimentalCalculator_;
public:
	typedef std::shared_ptr<NodeFactory> Ptr;

	NodeFactory(
			const HeurCalculator::Ptr& calculator,
			const HeurCalculator::Ptr& experimentalCalculator = HeurCalculator::Ptr()):
		numNodes_(0),
		calculator_(calculator),
		experimentalCalculator_(experimentalCalculator)
	{}
	Node::Ptr createNode(const Status & status, const Point &p,
			const Point &d, Node::Ptr ans)
	{
		return std::make_shared<Node>(
				status.state(), p, d, ans, 1,
				calculator_->calculateStatus(status, ans),
				experimentalCalculator_ ? experimentalCalculator_->calculateStatus(status, ans) : 0,
				++numNodes_);
	}
};


#endif /* NODEFACTORY_HPP_ */
