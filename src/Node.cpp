#include "Node.h"

using namespace std;

Node::Node():heur_(-1), stone_(-1), depth_(0), cost_(0)
{
}

Node::Node(const State &stones, int stone, const Point &d, Ptr ans,
		int c, int heur):
	ansector_(ans),
	state_(stones),
	heur_(heur),
	stone_(stone),
	d_(d)
{
	if (ans.get() == NULL)
	{
		depth_ = 0;
		cost_ = c;
	} else
	{
		depth_ = ans->depth_ + 1;
		cost_ = ans->cost_ + c;
	}
}

void Node::copy(const Node &other) {
	state_ = other.state();
	depth_ = other.depth();
	cost_ = other.cost();
	heur_ = other.heur();
	stone_ = other.stone();
	d_ = other.d();
	// ansector is not copied
}

deque<Node::Ptr> pathToRoot(Node::Ptr node)
{
	deque<Node::Ptr> result;
	while (node.get() != NULL)
	{
		result.push_front(node);
		node = node->ansector();
	}
	return result;
}

