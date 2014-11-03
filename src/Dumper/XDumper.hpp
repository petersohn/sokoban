#ifndef XDUMPER_H_
#define XDUMPER_H_

#include "Dumper/XML.hpp"
#include "Node.hpp"
#include "Status/Table.hpp"
#include "Dumper/Dumper.hpp"
#include "Mutexes.hpp"
#include <memory>
#include <map>
#include <memory>


class XDumper: public Dumper {
	XDumper(const XDumper &);
	const Table* table_;
	typedef std::map<Node::Ptr, std::shared_ptr<xml::XMLElement> > MapType;
	MapType elements_;
	std::string filename_;
	mutable MutexType xdumperMutex_;

	typedef std::shared_ptr<xml::XMLElement> ElementPtr;

	ElementPtr createDumpElement(const std::string &s);
	ElementPtr getElement(const Node::Ptr& node);
	ElementPtr doAddNode(const Node::Ptr& node);
public:
	XDumper(const char *filename);
	void initialStatus(const Status &status) override;
	void addNode(const Node::Ptr& node) override;
	void addToSolution(const Node::Ptr& node) override;
	void expand(const Node::Ptr& node) override;
	void push(const Node::Ptr& node) override;
	void reject(const Node::Ptr& node, const char *text) override;
	void clear();
	void save() override;
	void dump() const;
};

#endif /* XDUMPER_H_ */
