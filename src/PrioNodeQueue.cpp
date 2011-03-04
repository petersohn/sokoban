#include "PrioNodeQueue.h"
#include "CompareQueue.h"

typedef CompareQueue<Node> NodeComparQueue;

bool PrioNodeQueue::Compare::operator()(Node::Ptr n1, Node::Ptr n2) {
	if (a.get() == NULL || b.get() == NULL)
		return false;
	// Return true when n1 is worse than n2
	return NodeComparQueue::compareItems(comp_.begin(), comp_.end(), *n1, *n2) > 0;
}


void PrioNodeQueue::push(const Node::Ptr &node) {
	queue_.push(node);
}

Node::Ptr PrioNodeQueue::pop() {
	if (queue_.empty())
		return Node::Ptr();
	Node::Ptr result = queue_.top();
	queue_.pop();
	return result;
}

size_t PrioNodeQueue::size() {
	return queue_.size();
}
