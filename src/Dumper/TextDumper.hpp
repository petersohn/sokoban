#ifndef TEXTDUMPER_H_
#define TEXTDUMPER_H_

#include "Dumper/Dumper.hpp"
#include "Status/Table.hpp"
#include "Common.hpp"
#include <iostream>
#include <fstream>

class TextDumper: public Dumper {
	std::ofstream file_;
	const Table* table_;
public:
	TextDumper(const char *filename):
		file_(filename, std::ios::out | std::ios::trunc)
	{}
	void initialStatus(const Status &status) override;
	void addNode(const Node::Ptr& node) override;
	void addToSolution(const Node::Ptr& node) override;
	void expand(const Node::Ptr& node) override;
	void startPushing(const Node::Ptr& node) override;
	void push(const Node::Ptr& node) override;
	void reject(const Node::Ptr& node, const char *text) override;
	void save() override;
};

#endif /* TEXTDUMPER_H_ */
