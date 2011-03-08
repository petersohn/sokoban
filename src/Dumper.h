#ifndef DUMPER_H_
#define DUMPER_H_

#include "Table.h"
#include <boost/shared_ptr.hpp>

class Status;
class Node;

class Dumper {
public:
	typedef boost::shared_ptr<Dumper> Ptr;

	virtual void initialStatus(const Status &status) = 0;
	virtual void addNode(Node::Ptr node) = 0;
	virtual void addToSolution(Node::Ptr node) = 0;
	virtual void expand(Node::Ptr node) = 0;
	virtual void push(Node::Ptr node) = 0;
	virtual void reject(Node::Ptr node, const char *text) = 0;
	virtual ~Dumper() {}
};

#endif /* DUMPER_H_ */
