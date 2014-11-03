#include "Dumper/TextDumper.hpp"
#include "Status/Status.hpp"
#include "Node.hpp"
#include "Dumper/DumperFunctions.hpp"
#include <string>


void TextDumper::initialStatus(const Status &status) {
	boost::lock_guard<MutexType> lck(dumperMutex_);
	std::cerr << "Initial status" << std::endl;
	table_ = &status.table();
	dumpStatus(file_, status, "Initial status");
}

void TextDumper::addNode(const Node::Ptr& node) {
	boost::lock_guard<MutexType> lck(dumperMutex_);
	dumpNode(file_, *table_, *node, "Added");
}

void TextDumper::addToSolution(const Node::Ptr& /*node*/) {
// nothing to be done here
}

void TextDumper::expand(const Node::Ptr& node) {
	boost::lock_guard<MutexType> lck(dumperMutex_);
	file_ << std::endl << std::endl;
	dumpNode(file_, *table_, *node, "Expanded");
}

void TextDumper::push(const Node::Ptr& node) {
	boost::lock_guard<MutexType> lck(dumperMutex_);
	dumpNode(file_, *table_, *node, "Pushed out");
}

void TextDumper::reject(const Node::Ptr& node, const char *text) {
	boost::lock_guard<MutexType> lck(dumperMutex_);
	dumpNode(file_, *table_, *node, "Rejected: " + std::string(text));
}

void TextDumper::save() {
	boost::lock_guard<MutexType> lck(dumperMutex_);
	file_.flush();
}
