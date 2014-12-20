#ifndef SRC_DUMPER_STATISTICSDUMPER_HPP
#define SRC_DUMPER_STATISTICSDUMPER_HPP

#include "Dumper/Dumper.hpp"
#include "Dumper/Statistics.hpp"
#include <string>
#include <fstream>

class StatisticsDumper: public Dumper {
	std::ofstream file;
	Statistics statistics;

public:

	StatisticsDumper(const std::string& filename):
		file(filename, std::ios::out | std::ios::trunc),
		statistics(this->file, boost::posix_time::seconds(1))
	{}

	void initialStatus(const Status& status) override;
	void addNode(const std::shared_ptr<Node>& node) override;
	void addToSolution(const std::shared_ptr<Node>& node) override;
	void expand(const std::shared_ptr<Node>& node) override;
	void startPushing(const std::shared_ptr<Node>& node) override;
	void push(const std::shared_ptr<Node>& node) override;
	void reject(const std::shared_ptr<Node>& node, const char *text) override;
	void save() override;

};


#endif /* SRC_DUMPER_STATISTICSDUMPER_HPP */
