#ifndef SRC_DUMPER_STATISTICSDUMPER_HPP
#define SRC_DUMPER_STATISTICSDUMPER_HPP

#include "Dumper/Dumper.hpp"
#include <string>

class StatisticsDumper: public Dumper {
	using Statistics = std::map<std::string, unsigned>;

	Statistics statistics;
	std::string filename;

public:

	StatisticsDumper(std::string filename): 
		filename(std::move(filename))
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


#endif /* SRC_DUMPER_STATISTICSDUMPER_HPP */
