#ifndef PRIONODEQUEUE_H_
#define PRIONODEQUEUE_H_

#include "QueueInterfaces.h"
#include "Node.h"
#include <boost/function.hpp>
#include <vector>
#include <queue>

class PrioNodeQueue: public NodeQueue {
public:
	typedef boost::function<int(const Node&, const Node&)> NodeComparer;
private:
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
	virtual void push(const Node::Ptr &node);
	virtual Node::Ptr pop();
	virtual size_t size();
};

#endif /* PRIONODEQUEUE_H_ */
