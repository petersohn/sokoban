#include "XML.h"
#include <iostream>
#include <fstream>
#include <boost/format.hpp>

using namespace std;

namespace xml {

string XMLElement::toString() const {
	string attrString;
	for (map<string, string>::const_iterator it = attributes_.begin();
			it != attributes_.end(); ++it) {
		attrString += (boost::format("%s=\"%s\" ") % it->first % it->second).str();
	}
	string result = (boost::format("<%s %s>\n") % name_ % attrString).str();
	for (vector<Ptr>::const_iterator it = children_.begin();
			it != children_.end(); ++it)
		result += (*it)->toString();
	result += (boost::format("</%s>\n") % name_).str();
	return result;
}

string XMLText::toString() const {
	return data_;
}

void saveXMLFile(std::shared_ptr<XMLElement> rootElement, const char *filename, const char *dtdFilename) {
	assert(filename != NULL);
	assert(rootElement.get() != NULL);
	ofstream file(filename, ios::out | ios::trunc);
	file << "<?xml version=\"1.0\"?>" << endl;
	if (dtdFilename != NULL) {
		file << "<!DOCTYPE " << rootElement->name() << " SYSTEM \"" << dtdFilename << "\">" << std::endl;
	}
	file << rootElement->toString();
}

}
