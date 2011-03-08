#include "TextDumper.h"
#include "Status.h"
#include "Node.h"
#include "DumperFunctions.h"
#include <string>

void TextDumper::initialStatus(const Status &status) {
	table_ = status.tablePtr();
	dumpStatus(file_, status, "Initial status");
}

void TextDumper::addNode(Node::Ptr node) {
	dumpNode(file_, table_, node, "Added");
}

void TextDumper::addToSolution(Node::Ptr node) {
// nothing to be done here
}

void TextDumper::expand(Node::Ptr node) {
	dumpNode(file_, table_, node, "Expanded");
}

void TextDumper::push(Node::Ptr node) {
	dumpNode(file_, table_, node, "Pushed out");
}

void TextDumper::reject(Node::Ptr node, const char *text) {
	dumpNode(file_, table_, node, "Rejected: " + std::string(text));
}
