#include "Dumper/BestDumper.hpp"
#include "Dumper/DumperFunctions.hpp"
#include "Node.hpp"
#include "Status/Table.hpp"
#include "Status/Status.hpp"
#include <boost/format.hpp>

namespace sokoban {

void BestDumper::initialStatus(const Status& status)
{
    table = &status.table();
}

void BestDumper::addNode(const std::shared_ptr<Node>& node)
{
    ++nodeNumber;
    auto heur = node->heur();
    if (bestHeur < 0 || heur < bestHeur) {
        file << "-----------------------------------------------\n\n";
        dumpNode(file, *table, *node,
                (boost::format("Node %d = %d. Number of previous best = %d") %
                 nodeNumber % heur % bestNodeNumber).str());
        bestHeur = heur;
        bestNodeNumber = 1;
    } else if (heur == bestHeur) {
        dumpNode(file, *table, *node,
                (boost::format("Node %d = %d") %
                 nodeNumber % heur).str());
        ++bestNodeNumber;
    }
}

void BestDumper::addToSolution(const std::shared_ptr<Node>&)
{
}

void BestDumper::expand(const std::shared_ptr<Node>&)
{
}

void BestDumper::startPushing(const std::shared_ptr<Node>&)
{
}

void BestDumper::push(const std::shared_ptr<Node>&)
{
}

void BestDumper::reject(const std::shared_ptr<Node>&, const char *)
{
}

void BestDumper::save()
{
    file.flush();
}

} // namespace sokoban
