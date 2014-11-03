#include "Dumper/TextDumper.hpp"
#include "Status/Status.hpp"
#include "Node.hpp"
#include "Dumper/DumperFunctions.hpp"
#include <string>


void TextDumper::initialStatus(const Status &status) {
	std::cerr << "Initial status" << std::endl;
	table_ = &status.table();
	dumpStatus(file_, status, "Initial status");
}

void TextDumper::addNode(const Node::Ptr& node) {
	dumpNode(file_, *table_, *node, "Added");
}

void TextDumper::addToSolution(const Node::Ptr& /*node*/) {
// nothing to be done here
}

void TextDumper::expand(const Node::Ptr& node) {
	file_ << std::endl << std::endl;
	dumpNode(file_, *table_, *node, "Expanded");
}

void TextDumper::startPushing(const Node::Ptr& node) {
	dumpNode(file_, *table_, *node, "Pushing");
}

void TextDumper::push(const Node::Ptr& node) {
	dumpNode(file_, *table_, *node, "Pushed out");
}

void TextDumper::reject(const Node::Ptr& node, const char *text) {
	dumpNode(file_, *table_, *node, "Rejected: " + std::string(text));
}

void TextDumper::save() {
	file_.flush();
}
