/*
 * Node.cpp
 *
 *  Created on: 1 Feb 2011
 *      Author: eszabpt
 */

#include "Node.h"

using namespace std;

Node::Node():depth(0), cost(0)
{
}

Node::Node(const State &stones, int stone, const Point &d, NodeContainer ans, int c):
	ansector_(ans),state_(stones),stone_(stone),d_(d)
{
	if (ans.get() == NULL)
	{
		depth_ = 0;
		cost_ = c;
	} else
	{
		depth_ = ans->depth + 1;
		cost_ = ans->cost + c;
	}
	costFgv_ = cost + heur();
}

void Node::copy(const Node &other) {
	state_ = other.state();
	depth_ = other.depth();
	cost_ = other.cost();
	costFgv_ = other.costFgv();
	stone_ = other.stone();
	d_ = other.d();
	// ansector is not copied
}

deque<Node> Node::pathToRoot()
{
	deque<Node> result;
	Node *current = this;
	while (current != NULL)
	{
		result.push_front(*current);
		current = current->ansector().get();
	}
	return result;
}
