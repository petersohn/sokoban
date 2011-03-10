#ifndef TEXTDUMPER_H_
#define TEXTDUMPER_H_

#include "Dumper.h"
#include "Table.h"
#include <iostream>
#include <fstream>

class TextDumper: public Dumper {
	std::ofstream file_;
	FixedTable::Ptr table_;
public:
	TextDumper(const char *filename):
		file(filename, std::ios::out | std::ios::trunc)
	{}
	virtual void initialStatus(const Status &status);
	virtual void addNode(Node::Ptr node);
	virtual void addToSolution(Node::Ptr node);
	virtual void expand(Node::Ptr node);
	virtual void push(Node::Ptr node);
	virtual void reject(Node::Ptr node, const char *text);
	virtual void save();
};

#endif /* TEXTDUMPER_H_ */
