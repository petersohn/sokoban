#ifndef XDUMPER_H_
#define XDUMPER_H_

#include "XML.h"
#include "Node.h"
#include "Status/Table.h"
#include "Dumper.h"
#include "Mutexes.hpp"
#include <memory>
#include <map>
#include <memory>


class XDumper: public Dumper {
	XDumper(const XDumper &);
	FixedTable::Ptr table_;
	typedef std::map<Node::Ptr, std::shared_ptr<xml::XMLElement> > MapType;
	MapType elements_;
	std::string filename_;
	mutable MutexType xdumperMutex_;

	typedef std::shared_ptr<xml::XMLElement> ElementPtr;

	ElementPtr createDumpElement(const std::string &s);
	ElementPtr getElement(Node::Ptr node);
	ElementPtr doAddNode(Node::Ptr node);
public:
	XDumper(const char *filename);
	virtual void initialStatus(const Status &status);
	virtual void addNode(Node::Ptr node);
	virtual void addToSolution(Node::Ptr node);
	virtual void expand(Node::Ptr node);
	virtual void push(Node::Ptr node);
	virtual void reject(Node::Ptr node, const char *text);
	void clear();
	virtual void save();
	void dump() const;
};

#endif /* XDUMPER_H_ */
