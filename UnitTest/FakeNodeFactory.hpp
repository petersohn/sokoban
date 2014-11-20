#ifndef UNITTEST_FAKENODEFACTORY_HPP
#define UNITTEST_FAKENODEFACTORY_HPP

#include "Node.hpp"
#include "Status/Status.hpp"
#include <memory>

class FakeNodeFactory {
	int numNodes_ = 0;
public:
	FakeNodeFactory() = default;

	Node::Ptr createNode(
			const Status& status,
			const MoveDescriptor& moveDescriptor,
			const Node::Ptr& ancestor,
			int heur)
	{
		return std::make_shared<Node>(
				status.state(), moveDescriptor, ancestor, 1,
				heur, 0, ++numNodes_);
	}
};


#endif /* UNITTEST_FAKENODEFACTORY_HPP */
