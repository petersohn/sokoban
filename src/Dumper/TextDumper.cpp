#include "Dumper/TextDumper.h"
#include "Status/Status.h"
#include "Node.h"
#include "Dumper/DumperFunctions.h"
#include <string>


void TextDumper::initialStatus(const Status &status) {
	boost::lock_guard<MutexType> lck(dumperMutex_);
	std::cerr << "Initial status" << std::endl;
	table_ = status.tablePtr();
	dumpStatus(file_, status, "Initial status");
}

void TextDumper::addNode(Node::Ptr node) {
	boost::lock_guard<MutexType> lck(dumperMutex_);
	dumpNode(file_, table_, *node, "Added");
}

void TextDumper::addToSolution(Node::Ptr node) {
// nothing to be done here
}

void TextDumper::expand(Node::Ptr node) {
	boost::lock_guard<MutexType> lck(dumperMutex_);
	file_ << std::endl << std::endl;
	dumpNode(file_, table_, *node, "Expanded");
}

void TextDumper::push(Node::Ptr node) {
	boost::lock_guard<MutexType> lck(dumperMutex_);
	dumpNode(file_, table_, *node, "Pushed out");
}

void TextDumper::reject(Node::Ptr node, const char *text) {
	boost::lock_guard<MutexType> lck(dumperMutex_);
	dumpNode(file_, table_, *node, "Rejected: " + std::string(text));
}

void TextDumper::save() {
	boost::lock_guard<MutexType> lck(dumperMutex_);
	file_.flush();
}