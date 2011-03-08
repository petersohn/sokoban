#include "XDumper.h"
#include "DumperFunctions.h"
#include "Common.h"
#include <iostream>
#include <sstream>


XDumper::XDumper() {
	clear();
}

XDumper::ElementPtr XDumper::createDumpElement(const std::string &s) {
	boost::shared_ptr<xml::XMLText> dump(new xml::XMLText());
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
}

void XDumper::addNode(Node::Ptr node) {
	assert(node.get() != NULL);
	ElementPtr elem(new xml::XMLElement());
	elem->name("node");
	std::stringstream ss;
	dumpNode(ss, table_, *node);
	elem->children().push_back(createDumpElement(ss.str()));
	elem->attributes().insert(make_pair("stone", pointStr(node->from())));
	elem->attributes().insert(make_pair("d", direction(node->d())));
	elem->attributes().insert(make_pair("cost", toStr(node->cost())));
	elem->attributes().insert(make_pair("heur", toStr(node->heur())));
	elem->attributes().insert(make_pair("costFgv", toStr(node->costFgv())));
	elements_[node] = elem;
	elements_[node->ansector()]->children().push_back(elem);
}

void XDumper::addToSolution(Node::Ptr node) {
	elements_[node]->attributes()["part-of-solution"] = "yes";
}

void XDumper::expand(Node::Ptr node) {
	elements_[node]->attributes()["expanded"] = "normal";
}

void XDumper::push(Node::Ptr node) {
	elements_[node]->attributes()["expanded"] = "pushed";
}

void XDumper::reject(Node::Ptr node, const char *text) {
	elements_[node]->attributes()["rejected"] = text;
}

void XDumper::clear() {
	elements_.clear();
	boost::shared_ptr<xml::XMLElement> root(new xml::XMLElement());
	root->name("root-node");
	elements_.insert(std::make_pair(Node::Ptr(), root));
}

void XDumper::save(const char *filename) const {
	std::cerr << elements_.size() << std::endl;
	MapType::const_iterator it = elements_.find(Node::Ptr());
	assert(it != elements_.end());
	saveXMLFile(it->second, filename, "dump.dtd");
}

void XDumper::dump() const {
	for (MapType::const_iterator it = elements_.begin(); it != elements_.end(); ++it) {
		std::cerr << "Node" << std::endl;
		std::cerr << it->second->toString() << std::endl << std::endl;
	}
}


