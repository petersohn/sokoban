#include "Dumper/StatisticsDumper.hpp"

void StatisticsDumper::initialStatus(const Status&)
{
	statistics.init();
}

void StatisticsDumper::addNode(const Node::Ptr&)
{
	statistics.update("add node");
}

void StatisticsDumper::addToSolution(const Node::Ptr&)
{
}

void StatisticsDumper::expand(const Node::Ptr&)
{
	statistics.update("expand");
}

void StatisticsDumper::startPushing(const Node::Ptr&)
{
	statistics.update("push");
}

void StatisticsDumper::push(const Node::Ptr&)
{
	statistics.update("push node");
}

void StatisticsDumper::reject(const Node::Ptr&, const char *text)
{
	statistics.update(std::string{"reject: "} + text);
}

void StatisticsDumper::save()
{
	statistics.printGlobal();
}



