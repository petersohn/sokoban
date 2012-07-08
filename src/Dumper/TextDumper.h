#ifndef TEXTDUMPER_H_
#define TEXTDUMPER_H_

#include "Dumper/Dumper.h"
#include "Status/Table.h"
#include "Common.h"
#include "Mutexes.hpp"
#include <iostream>
#include <fstream>

class TextDumper: public Dumper {
	std::ofstream file_;
	FixedTable::Ptr table_;
	mutable MutexType dumperMutex_;
public:
	TextDumper(const char *filename):
		file_(filename, std::ios::out | std::ios::trunc),
		MUTEX_DECL(dumperMutex_)
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
