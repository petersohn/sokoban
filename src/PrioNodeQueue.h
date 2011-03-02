#ifndef PRIONODEQUEUE_H_
#define PRIONODEQUEUE_H_

#include "QueueInterfaces.h"
#include "Node.h"
#include "CompareQueue.h"
#include <boost/function.hpp>
#include <vector>
#include <queue>

typedef Queue<Node::Ptr> NodeQueue;

class PrioNodeQueue: public NodeQueue {
public:
	typedef boost::function<int(const Node&, const Node&)> NodeComparer;
private:
	typedef CompareQueue<Node> NodeComparQueue;
	class Compare {
		std::vector<NodeComparer> comp_;
	public:
		Compare(const std::vector<NodeComparer> &comp):
			comp_(comp) {}
		bool operator()(Node::Ptr n1, Node::Ptr n2);
	};
	std::priority_queue<Node::Ptr, std::vector<Node::Ptr>, Compare> queue_;
public:
	PrioNodeQueue(const std::vector<NodeComparer> &compare):
		queue_(Compare(compare_)) {}
	void push(const Node::Ptr &node);
	Node::Ptr pop();
};

#endif /* PRIONODEQUEUE_H_ */
