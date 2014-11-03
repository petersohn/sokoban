#ifndef DUMPER_H_
#define DUMPER_H_

#include "Status/Table.hpp"
#include <memory>
#include "Node.hpp"

class Status;

class Dumper {
public:
	typedef std::shared_ptr<Dumper> Ptr;

	virtual void initialStatus(const Status &status) = 0;
	virtual void addNode(const Node::Ptr& node) = 0;
	virtual void addToSolution(const Node::Ptr& node) = 0;
	virtual void expand(const Node::Ptr& node) = 0;
	virtual void push(const Node::Ptr& node) = 0;
	virtual void reject(const Node::Ptr& node, const char *text) = 0;
	virtual void save() = 0;
	virtual ~Dumper() {}
};

typedef std::function<Dumper::Ptr()> DumperFactory;

#endif /* DUMPER_H_ */
