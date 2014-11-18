#ifndef PRIONODEQUEUE_H_
#define PRIONODEQUEUE_H_

#include "Node.hpp"
#include "Mutexes.hpp"
#include "CompareQueue.hpp"
#include <queue>
#include <boost/thread/locks.hpp>

class PrioNodeQueue {
	using Compare = CompareQueue<Node::Ptr>;
	std::priority_queue<Node::Ptr, std::vector<Node::Ptr>, Compare> queue_;
public:
	PrioNodeQueue(Compare compare):
		queue_(compare)
	{}

	void push(const Node::Ptr &node) { queue_.push(node); }

	Node::Ptr pop()
	{
		if (queue_.empty())
			return Node::Ptr();
		Node::Ptr result = queue_.top();
		queue_.pop();
		return result;
	}

	Node::Ptr peek() const
	{
		if (queue_.empty())
			return Node::Ptr();
		Node::Ptr result = queue_.top();
		return result;
	}

	size_t size() const { return queue_.size(); }
};

typedef std::function<std::shared_ptr<PrioNodeQueue>()> QueueFactory;

#endif /* PRIONODEQUEUE_H_ */
