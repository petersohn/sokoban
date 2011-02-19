#include "XML.h"
#include <iostream>
#include <fstream>
#include <boost/format.hpp>


using namespace std;

string XMLElement::toString() {
	string result = (boost::format("<%s>\n") % name_).str();
	for (vector<Ptr>::iterator it = children_.begin();
			it != children_.end(); ++it)
		result += (*it)->toString();
	result += (boost::format("</%s>\n") % name_).str();
	return result;
}

string XMLText::toString() {
	return data_;
}

void saveXMLFile(boost::shared_ptr<XMLElement> rootElement, const char *filename, const char *dtdFilename) {
	assert(filename != NULL);
	assert(rootElement.get() != NULL);
	ofstream file(filename, ios::out | ios::trunc);
	file << "<?xml version=\"1.0\"?>" << endl;
	if (dtdFilename != NULL) {
		file << "<!DOCTYPE " << rootElement->name() << " SYSTEM \"" << dtdFilename << "\">" << std::endl;
	}
	file << rootElement->toString();
}
