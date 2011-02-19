#ifndef XML_H_
#define XML_H_

#include <map>
#include <vector>
#include <string>
#include <boost/shared_ptr.hpp>

class XMLNode {
public:
	typedef boost::shared_ptr<XMLNode> Ptr;
	virtual std::string toString() = 0;
	virtual ~XMLNode() {}
};

class XMLElement: public XMLNode {
	std::string name_;
	std::map<std::string, std::string> attributes_;
	std::vector<Ptr> children_;
public:
	std::map<std::string, std::string> attributes() { return attributes_; }
	const std::map<std::string, std::string> attributes() const { return attributes_; }
	std::vector<Ptr> children() { return children_; }
	const std::vector<Ptr> children() const { return children_; }
	const std::string &name() { return name_; }
	void name(const std::string name) { name_ = name; }

	virtual std::string toString();
};

class XMLText: public XMLNode {
	std::string data_;
public:
	const std::string &data() { return data_; }
	void data(const std::string data) { data_ = data; }
	virtual std::string toString();
};


void saveXMLFile(XMLElement::Ptr rootElement, const char *filename, const char *dtdFilename = NULL);

#endif /* XML_H_ */
