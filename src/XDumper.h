#ifndef XDUMPER_H_
#define XDUMPER_H_

#include "XML.h"
#include "Node.h"
#include "Table.h"
#include <boost/shared_ptr.hpp>
#include <map>
#include <memory>


class XDumper {
	XDumper(const XDumper &);
	FixedTable::Ptr table_;
	typedef std::map<Node::Ptr, boost::shared_ptr<xml::XMLElement> > MapType;
	MapType elements_;
public:
	XDumper(FixedTable::Ptr table);
	void addNode(Node::Ptr node);
	void addToSolution(Node::Ptr node);
	void expand(Node::Ptr node);
	void push(Node::Ptr node);
	void reject(Node::Ptr node, const std::string &text);
	void clear();
	void save(const char *filename) const;
	void dump() const;
};

#endif /* XDUMPER_H_ */
