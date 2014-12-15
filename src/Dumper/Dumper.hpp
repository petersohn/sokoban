#ifndef DUMPER_H_
#define DUMPER_H_

#include "Status/Table.hpp"
#include <memory>

class Status;
class Node;

class Dumper {
public:
	typedef std::shared_ptr<Dumper> Ptr;

	virtual void initialStatus(const Status &status) = 0;
	virtual void addNode(const std::shared_ptr<Node>& node) = 0;
	virtual void addToSolution(const std::shared_ptr<Node>& node) = 0;
	virtual void expand(const std::shared_ptr<Node>& node) = 0;
	virtual void startPushing(const std::shared_ptr<Node>& node) = 0;
	virtual void push(const std::shared_ptr<Node>& node) = 0;
	virtual void reject(const std::shared_ptr<Node>& node, const char *text) = 0;
	virtual void save() = 0;
	virtual ~Dumper() {}
};

typedef std::function<Dumper::Ptr()> DumperFactory;

#endif /* DUMPER_H_ */
