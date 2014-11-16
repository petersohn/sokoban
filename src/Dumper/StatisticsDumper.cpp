#include "Dumper/StatisticsDumper.hpp"
#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/map.hpp>
#include <boost/lexical_cast.hpp>

const boost::posix_time::time_duration StatisticsDumper::printInterval =
		boost::posix_time::seconds(1);

void StatisticsDumper::update(const std::string& key)
{

	++globalStatistics[key];
	++currentStatistics[key];

	auto currentTime = boost::posix_time::microsec_clock::universal_time();
	if (currentTime > nextPrintTime) {
		print(nextPrintTime, currentStatistics);
		nextPrintTime += printInterval;

		for (auto& value: currentStatistics) {
			value.second = 0;
		}
	}
}

void StatisticsDumper::print(boost::posix_time::ptime time, const Statistics& statistics)
{
	if (!headerPrinted) {
		file << "time;" <<
			boost::algorithm::join(statistics | boost::adaptors::map_keys, ";") <<
			std::endl;

		headerPrinted = true;
	}

	if (time != boost::posix_time::not_a_date_time) {
		file << time - startTime;
	}

	file << ";" << boost::algorithm::join(statistics | boost::adaptors::map_values |
				boost::adaptors::transformed(
					boost::lexical_cast<std::string, unsigned>), ";") <<
			std::endl;
}

void StatisticsDumper::initialStatus(const Status&)
{
	startTime = boost::posix_time::microsec_clock::universal_time();
	nextPrintTime = startTime + printInterval;
}

void StatisticsDumper::addNode(const Node::Ptr&)
{
	update("add node");
}

void StatisticsDumper::addToSolution(const Node::Ptr&)
{
}

void StatisticsDumper::expand(const Node::Ptr&)
{
	update("expand");
}

void StatisticsDumper::startPushing(const Node::Ptr&)
{
	update("push");
}

void StatisticsDumper::push(const Node::Ptr&)
{
	update("push node");
}

void StatisticsDumper::reject(const Node::Ptr&, const char *text)
{
	update(std::string{"reject: "} + text);
}

void StatisticsDumper::save()
{
	print(boost::posix_time::not_a_date_time, globalStatistics);
}



