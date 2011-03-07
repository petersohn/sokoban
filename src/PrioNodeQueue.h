#ifndef PRIONODEQUEUE_H_
#define PRIONODEQUEUE_H_

#include "QueueInterfaces.h"
#include "Node.h"
#include <queue>

template <class Compare>
class PrioNodeQueue: public NodeQueue {
	std::priority_queue<Node::Ptr, std::vector<Node::Ptr>, Compare> queue_;
public:
	PrioNodeQueue(Compare compare):
		queue_(compare)
	{}
	virtual void push(const Node::Ptr &node);
	virtual Node::Ptr pop();
	virtual size_t size();
};

template <class Compare>
void PrioNodeQueue<Compare>::push(const Node::Ptr &node) {
	queue_.push(node);
}

template <class Compare>
Node::Ptr PrioNodeQueue<Compare>::pop() {
	if (queue_.empty())
		return Node::Ptr();
	Node::Ptr result = queue_.top();
	queue_.pop();
	return result;
}

template <class Compare>
size_t PrioNodeQueue<Compare>::size() {
	return queue_.size();
}


#endif /* PRIONODEQUEUE_H_ */
