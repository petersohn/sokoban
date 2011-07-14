#ifndef PRIONODEQUEUE_H_
#define PRIONODEQUEUE_H_

#include "QueueInterfaces.h"
#include "Node.h"
#include "Common.h"
#include <queue>
#include <boost/thread/locks.hpp>

template <class Compare>
class PrioNodeQueue: public NodeQueue {
	std::priority_queue<Node::Ptr, std::vector<Node::Ptr>, Compare> queue_;
	mutable SharedMutexType mtx_;
public:
	PrioNodeQueue(Compare compare):
		queue_(compare)
	{}
	virtual void push(const Node::Ptr &node);
	virtual Node::Ptr pop();
	virtual Node::Ptr peek();
	virtual size_t size() const;
};

template <class Compare>
void PrioNodeQueue<Compare>::push(const Node::Ptr &node) {
	boost::lock_guard<SharedMutexType> lck(mtx_);
	queue_.push(node);
}

template <class Compare>
Node::Ptr PrioNodeQueue<Compare>::pop() {
	boost::lock_guard<SharedMutexType> lck(mtx_);
	if (queue_.empty())
		return Node::Ptr();
	Node::Ptr result = queue_.top();
	queue_.pop();
	return result;
}

template <class Compare>
Node::Ptr PrioNodeQueue<Compare>::peek() {
	boost::shared_lock<SharedMutexType> lck(mtx_);
	if (queue_.empty())
		return Node::Ptr();
	Node::Ptr result = queue_.top();
	return result;
}

template <class Compare>
size_t PrioNodeQueue<Compare>::size() const {
	boost::shared_lock<SharedMutexType> lck(mtx_);
	return queue_.size();
}


#endif /* PRIONODEQUEUE_H_ */
