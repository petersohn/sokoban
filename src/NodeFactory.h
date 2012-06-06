#ifndef NODEFACTORY_HPP_
#define NODEFACTORY_HPP_

#include "Node.h"
#include "HeurCalculator.h"
#include "Status.h"
#include <memory>

class NodeFactory {
	int numNodes_;
	HeurCalculator::Ptr calculator_;
public:
	typedef std::shared_ptr<NodeFactory> Ptr;

	NodeFactory(HeurCalculator::Ptr calculator):
		numNodes_(0),
		calculator_(calculator)
	{}
	Node::Ptr createNode(const Status & status, const Point &p,
			const Point &d, Node::Ptr ans)
	{
		Node::Ptr result(new Node(status.state(), p, d, ans, 1, calculator_->calculateStatus(status), ++numNodes_));
		return result;
	}
};


#endif /* NODEFACTORY_HPP_ */
