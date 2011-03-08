#ifndef XDUMPER_H_
#define XDUMPER_H_

#include "XML.h"
#include "Node.h"
#include "Table.h"
#include "Dumper.h"
#include <boost/shared_ptr.hpp>
#include <map>
#include <memory>


class XDumper: public Dumper {
	XDumper(const XDumper &);
	FixedTable::Ptr table_;
	typedef std::map<Node::Ptr, boost::shared_ptr<xml::XMLElement> > MapType;
	MapType elements_;

	typedef boost::shared_ptr<xml::XMLElement> ElementPtr;

	ElementPtr createDumpElement(const std::string &s);
public:
	XDumper();
	virtual void initialStatus(const Status &status);
	virtual void addNode(Node::Ptr node);
	virtual void addToSolution(Node::Ptr node);
	virtual void expand(Node::Ptr node);
	virtual void push(Node::Ptr node);
	virtual void reject(Node::Ptr node, const char *text);
	void clear();
	void save(const char *filename) const;
	void dump() const;
};

#endif /* XDUMPER_H_ */
