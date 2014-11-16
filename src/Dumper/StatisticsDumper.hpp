#ifndef SRC_DUMPER_STATISTICSDUMPER_HPP
#define SRC_DUMPER_STATISTICSDUMPER_HPP

#include "Dumper/Dumper.hpp"
#include "Mutexes.hpp"
#include <string>
#include <fstream>
#include <boost/date_time/posix_time/posix_time.hpp>

class StatisticsDumper: public Dumper {
	using Statistics = std::map<std::string, unsigned>;

	Statistics currentStatistics, globalStatistics;
	std::ofstream file;
	bool headerPrinted = false;
	boost::posix_time::ptime startTime, nextPrintTime;

	void update(const std::string& key);
	void print(boost::posix_time::ptime time, const Statistics& statistics);

	static const boost::posix_time::time_duration printInterval;
public:

	StatisticsDumper(const std::string& filename):
		file(filename, std::ios::out | std::ios::trunc)
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
