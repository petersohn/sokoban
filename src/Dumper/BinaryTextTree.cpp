#include "Dumper/BinaryTextTree.h"

using namespace xml;

xml::XMLNode::Ptr toXml(const BinaryTextTree::ConstPtr& tree, const std::string& name = "node")
{
	std::shared_ptr<XMLElement> element = std::make_shared<XMLElement>();
	element->name(name);
	element->attributes().insert(std::make_pair("value", tree->value()));
	if (tree->falseChild()) {
		element->children().push_back(toXml(tree->falseChild()), "falseNode");
	}
	if (tree->trueChild()) {
		element->children().push_back(toXml(tree->trueChild()), "trueNode");
	}
}
