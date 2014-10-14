#ifndef TEXTDUMPER_H_
#define TEXTDUMPER_H_

#include "Dumper/Dumper.hpp"
#include "Status/Table.hpp"
#include "Common.hpp"
#include "Mutexes.hpp"
#include <iostream>
#include <fstream>

class TextDumper: public Dumper {
	std::ofstream file_;
	const Table* table_;
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
