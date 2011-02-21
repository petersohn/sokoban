#include "XDumper.h"
#include "Dumper.h"
#include "Common.h"
#include <iostream>
#include <sstream>


XDumper::XDumper(FixedTable::Ptr table):table_(table) {
	clear();
}

void XDumper::addNode(Node::Ptr node) {
	assert(node.get() != NULL);
	std::stringstream ss;
	dumpNode(ss, table_, *node);
	boost::shared_ptr<xml::XMLText> dump(new xml::XMLText());
	dump->data(ss.str());
	boost::shared_ptr<xml::XMLElement> dumpElem(new xml::XMLElement());
	dumpElem->name("dump");
	dumpElem->children().push_back(dump);
	boost::shared_ptr<xml::XMLElement> elem(new xml::XMLElement());
	elem->name("node");
	elem->children().push_back(dumpElem);
	elem->attributes().insert(make_pair("stone", toStr(node->stone())));
	elem->attributes().insert(make_pair("d", direction(node->d())));
	elem->attributes().insert(make_pair("cost", toStr(node->cost())));
	elem->attributes().insert(make_pair("heur", toStr(node->heur())));
	elem->attributes().insert(make_pair("costFgv", toStr(node->costFgv())));
	elements_[node] = elem;
	elements_[node->ansector()]->children().push_back(elem);
}

void XDumper::addToSolution(Node::Ptr node) {
	elements_[node]->attributes()["expanded"] = "part-of-solution";
}

void XDumper::expand(Node::Ptr node) {
	elements_[node]->attributes()["expanded"] = "yes";
}

void XDumper::reject(Node::Ptr node, const std::string &text) {
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


