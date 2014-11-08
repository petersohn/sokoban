#include "Dumper/XDumper.hpp"
#include "Dumper/DumperFunctions.hpp"
#include "Status/Status.hpp"
#include <iostream>
#include <sstream>
#include <boost/lexical_cast.hpp>


XDumper::XDumper(const char *filename):
		filename_(filename)
{
	clear();
}

XDumper::ElementPtr XDumper::createDumpElement(const std::string &s) {
	std::shared_ptr<xml::XMLText> dump(new xml::XMLText());
	dump->data(s);
	ElementPtr dumpElem(new xml::XMLElement());
	dumpElem->name("dump");
	dumpElem->children().push_back(dump);
	return dumpElem;
}

void XDumper::initialStatus(const Status &status) {
	std::stringstream ss;
	dumpStatus(ss, status);
	elements_[Node::Ptr()]->children().push_back(createDumpElement(ss.str()));
	table_ = &status.table();
}

void  XDumper::addNode(const Node::Ptr& node)
{
	doAddNode(node);
}

XDumper::ElementPtr XDumper::doAddNode(const Node::Ptr& node)
{
	ElementPtr elem(new xml::XMLElement());
	elem->name("node");
	std::stringstream ss;
	dumpNode(ss, *table_, *node);
	elem->children().push_back(createDumpElement(ss.str()));
	elem->attributes().insert(make_pair("stone", pointStr(node->from())));
	elem->attributes().insert(make_pair("d", direction(node->d())));
	elem->attributes().insert(make_pair("cost", boost::lexical_cast<std::string>(node->cost())));
	elem->attributes().insert(make_pair("heur", boost::lexical_cast<std::string>(node->heur())));
	elem->attributes().insert(make_pair("costFgv", boost::lexical_cast<std::string>(node->costFgv())));
	elements_[node] = elem;
	elements_[node->ancestor()]->children().push_back(elem);
	return elem;
}

XDumper::ElementPtr XDumper::getElement(const Node::Ptr& node)
{
	MapType::iterator it = elements_.find(node);
	if (it == elements_.end())
		return doAddNode(node);
	return it->second;
}

void XDumper::addToSolution(const Node::Ptr& node) {
	getElement(node)->attributes()["part-of-solution"] = "yes";
}

void XDumper::expand(const Node::Ptr& node) {
	getElement(node)->attributes()["expanded"] = "normal";
}

void XDumper::startPushing(const Node::Ptr& node) {
	getElement(node)->attributes()["expanded"] = "pushed";
}

void XDumper::push(const Node::Ptr& node) {
	getElement(node)->attributes()["expanded"] = "push-node";
}

void XDumper::reject(const Node::Ptr& node, const char *text) {
	getElement(node)->attributes()["rejected"] = text;
}

void XDumper::clear() {
	elements_.clear();
	std::shared_ptr<xml::XMLElement> root(new xml::XMLElement());
	root->name("root-node");
	elements_.insert(std::make_pair(Node::Ptr(), root));
}

void XDumper::save() {
	std::cerr << elements_.size() << std::endl;
	MapType::const_iterator it = elements_.find(Node::Ptr());
	assert(it != elements_.end());
	saveXMLFile(it->second, filename_.c_str(), "dump.dtd");
}

void XDumper::dump() const {
	for (MapType::const_iterator it = elements_.begin(); it != elements_.end(); ++it) {
		std::cerr << "Node" << std::endl;
		std::cerr << it->second->toString() << std::endl << std::endl;
	}
}


