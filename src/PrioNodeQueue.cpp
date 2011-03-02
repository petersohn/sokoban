#include "PrioNodeQueue.h"


bool PrioNodeQueue::Compare::operator()(Node::Ptr n1, Node::Ptr n2) {
	if (a.get() == NULL || b.get() == NULL)
		return false;
	return *a < *b;
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
