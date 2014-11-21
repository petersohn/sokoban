#ifndef UNITTEST_FAKENODEFACTORY_HPP
#define UNITTEST_FAKENODEFACTORY_HPP

#include "Node.hpp"
#include <memory>

class FakeNodeFactory {
	int numNodes_ = 0;
public:
	FakeNodeFactory() = default;

	Node::Ptr createNode( const Node::Ptr& ancestor, int heur)
	{
		return std::make_shared<Node>(
				State{}, MoveDescriptor{Point{0, 0}, Point{0, 0}}, ancestor, 1,
				heur, 0, ++numNodes_);
	}
};


#endif /* UNITTEST_FAKENODEFACTORY_HPP */
