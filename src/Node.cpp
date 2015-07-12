#include "Node.hpp"

using namespace std;

Node::Node(const State& stones, const MoveDescriptor& moveDescriptor,
        std::shared_ptr<Node> ancestor,
        float c, float heur, float experimentalHeur, int time):
    ancestor_(ancestor),
    state_(stones),
    heur_(heur),
    experimentalHeur_(experimentalHeur),
    moveDescriptor_(moveDescriptor),
    time_(time)
{
    if (ancestor.get() == nullptr)
    {
        depth_ = 0;
        cost_ = c;
    } else
    {
        depth_ = ancestor->depth_ + 1;
        cost_ = ancestor->cost_ + c;
    }
}

deque<std::shared_ptr<Node>> pathToBase(std::shared_ptr<Node> node, std::shared_ptr<Node> base)
{
    deque<std::shared_ptr<Node>> result;
    while (node.get() != nullptr && node != base)
    {
        result.push_front(node);
        node = node->ancestor();
    }
    return result;
}

