#include "Dumper/StatisticsDumper.hpp"

void StatisticsDumper::initialStatus(const Status&)
{
	statistics.init();
}

void StatisticsDumper::addNode(const std::shared_ptr<Node>&)
{
	statistics.update("add node");
}

void StatisticsDumper::addToSolution(const std::shared_ptr<Node>&)
{
}

void StatisticsDumper::expand(const std::shared_ptr<Node>&)
{
	statistics.update("expand");
}

void StatisticsDumper::startPushing(const std::shared_ptr<Node>&)
{
	statistics.update("push");
}

void StatisticsDumper::push(const std::shared_ptr<Node>&)
{
	statistics.update("push node");
}

void StatisticsDumper::reject(const std::shared_ptr<Node>&, const char *text)
{
	statistics.update(std::string{"reject: "} + text);
}

void StatisticsDumper::save()
{
	statistics.printGlobal();
}



