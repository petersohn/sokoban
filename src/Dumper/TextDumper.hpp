#ifndef TEXTDUMPER_H_
#define TEXTDUMPER_H_

#include "Dumper/Dumper.hpp"
#include "Status/Table.hpp"
#include <iostream>
#include <fstream>
#include <regex>
#include <boost/variant.hpp>

class TextDumper: public Dumper {
public:
	struct NoFilter {};
	using FilterType = boost::variant<NoFilter, std::string, std::regex>;
private:

	std::ofstream file_;
	const Table* table_;

	FilterType filter_;

	void dump(const Node& node, const std::string& text);
public:
	TextDumper(const std::string& filename, const FilterType& filter):
		file_(filename, std::ios::out | std::ios::trunc),
		filter_(filter)
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
