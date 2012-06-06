#ifndef DUMPER_H_
#define DUMPER_H_

#include "Table.h"
#include <memory>
#include "Node.h"

class Status;

class Dumper {
public:
	typedef std::shared_ptr<Dumper> Ptr;

	virtual void initialStatus(const Status &status) = 0;
	virtual void addNode(Node::Ptr node) = 0;
	virtual void addToSolution(Node::Ptr node) = 0;
	virtual void expand(Node::Ptr node) = 0;
	virtual void push(Node::Ptr node) = 0;
	virtual void reject(Node::Ptr node, const char *text) = 0;
	virtual void save() = 0;
	virtual ~Dumper() {}
};

#endif /* DUMPER_H_ */
