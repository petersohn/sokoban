#include "Node.h"

using namespace std;

//Node::Node():heur_(-1), depth_(0), cost_(0), time_(0)
//{
//}

Node::Node(const State &stones, const MoveDescriptor& moveDescriptor, Ptr ans,
		int c, int heur, int experimentalHeur, int time):
	ancestor_(ans),
	state_(stones),
	heur_(heur),
	experimentalHeur_(experimentalHeur),
	moveDescriptor_(moveDescriptor),
	time_(time)
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

deque<Node::Ptr> pathToBase(Node::Ptr node, Node::Ptr base)
{
	deque<Node::Ptr> result;
	while (node.get() != NULL && node != base)
	{
		result.push_front(node);
		node = node->ancestor();
	}
	return result;
}

