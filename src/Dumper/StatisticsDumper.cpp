#include "Dumper/StatisticsDumper.hpp"
#include <fstream>
#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/map.hpp>
#include <boost/lexical_cast.hpp>

void StatisticsDumper::initialStatus(const Status&)
{
}

void StatisticsDumper::addNode(const Node::Ptr&)
{
	++statistics["add node"];
}

void StatisticsDumper::addToSolution(const Node::Ptr&)
{
}

void StatisticsDumper::expand(const Node::Ptr&)
{
	++statistics["expand"];
}

void StatisticsDumper::startPushing(const Node::Ptr&)
{
	++statistics["push"];
}

void StatisticsDumper::push(const Node::Ptr&)
{
	++statistics["push node"];
}

void StatisticsDumper::reject(const Node::Ptr&, const char *text)
{
	++statistics[std::string{"reject: "} + text];
}

void StatisticsDumper::save()
{
	std::ofstream file{filename, std::ios::out | std::ios::trunc};
	file << boost::algorithm::join(statistics | boost::adaptors::map_keys, ";") <<
		"\n" <<
		boost::algorithm::join(statistics | boost::adaptors::map_values |
				boost::adaptors::transformed(
					boost::lexical_cast<std::string, unsigned>), ";") << "\n";
}



