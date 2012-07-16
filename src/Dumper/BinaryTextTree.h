#ifndef BINARYTEXTTREE_H_
#define BINARYTEXTTREE_H_

#include <memory>
#include <algorithm>
#include <string>
#include "XML.h"

class BinaryTextTree {
public:
	typedef std::unique_ptr<BinaryTextTree> Ptr;
	typedef std::unique_ptr<const BinaryTextTree> ConstPtr;
private:
	std::string value_;
	Ptr falseChild_;
	Ptr trueChild_;
public:
	BinaryTextTree(const std::string& value):
		value_(value)
	{}
	BinaryTextTree(std::string&& value):
		value_(std::move(value))
	{}

	const ConstPtr& falseChild() const { return falseChild_; }
	const ConstPtr& trueChild() const { return trueChild_; }
	Ptr& falseChild() { return falseChild_; }
	Ptr& trueChild() { return trueChild_; }
	const std::string& value() const { return value_; }
};

xml::XMLNode::Ptr toXml(const BinaryTextTree::ConstPtr& tree, const std::string& name = "node");


#endif /* BINARYTEXTTREE_H_ */
