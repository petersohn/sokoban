#include "HeurCalculator/AdvancedHeurCalculator.hpp"
#include "Status/Status.hpp"
#include "Status/StatusUtils.hpp"
#include "Node.hpp"
#include "Solver.hpp"

#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/range/algorithm.hpp>

#include <algorithm>
#include <string>
#include <vector>


/* AdvancedStoneCalculator::HeurDumper */

void AdvancedStoneCalculator::HeurDumper::open()
{
    if (!file_.is_open())
        file_.open(filename_, std::ofstream::out | std::ofstream::trunc);
}

void AdvancedStoneCalculator::HeurDumper::dumpPartition(const Table& table,
        const Partition& partition)
{
    open();
    Status s(table);
    s.addStone(partition.pos);
    s.currentPos(Point(-1, -1));
    dumpStatus(file_, s,
            (boost::format("Partition (%d)") % partition.heur).str(),
            &partition.reachable);
    file_ << std::endl;
}

void AdvancedStoneCalculator::HeurDumper::printText(const char *text)
{
    open();
    file_ << text << std::endl;
}


/* AdvancedStoneCalculator */

void AdvancedStoneCalculator::init(const Table& table, const Solver& solver,
        std::size_t reverseSearchMaxDepth,
        const boost::optional<std::string>& filename)
{
    Array<std::string> dump(table.width(), table.height());
    std::vector<Partition> dumpPartitions;
    partitions_.reset(table.width(), table.height());
    for (Point  p: arrayRange(table)) {
        if (table.wall(p)) {
            dump[p] = "*";
            continue;
        }
        initPartitions(table, p, solver, reverseSearchMaxDepth);
        dump[p] =
                partitions_[p].empty() ? "#" :
                partitions_[p].size() > 1 ? "?" :
                boost::lexical_cast<std::string>(partitions_[p][0].heur);
    }
    if (filename) {
        HeurDumper dumper{*filename};
        dumper.printText("Heuristics table:");
        dumper.dumpArray(dump);
        dumper.printText("\nPartitions:");
        for (Point  p: arrayRange(table)) {
            if (partitions_[p].size() > 1) {
                for (const Partition& partition: partitions_[p]) {
                    dumper.dumpPartition(table, partition);
                }
            }
        }
    }
}

void AdvancedStoneCalculator::initPartitions(const Table& table, Point  p,
        const Solver& solver, std::size_t reverseSearchMaxDepth)
{
    State state;
    state.addStone(p);
    std::vector<Status> parts = getPartitions(table, state,
            reverseSearchMaxDepth);
    for (Status& status: parts) {
        Partition partition(table.width(), table.height());
        partition.pos = p;
        partition.heur = -1;
        partition.reachable = status.reachableArray();
        if (p == table.destination())
            partition.heur = 0;
        else {
            std::deque<std::shared_ptr<Node>> res =
                    solver.solve(std::move(status));
            if (!res.empty())
                partition.heur = res.back()->cost();
        }
        partitions_[p].push_back(std::move(partition));
    }
}

float AdvancedStoneCalculator::operator()(const Status& status, Point p) const
{
    std::vector<Partition>::const_iterator it;
    // If the current position equals p, then partitions can't be used. Use the
    // minimal non-negative partition's value instead
    if (status.currentPos() == p) {
        auto minElement = boost::min_element(partitions_[p],
                [](const Partition& left, const Partition& right)
                {
                    return static_cast<unsigned>(left.heur) <
                        static_cast<unsigned>(right.heur);
                });
        return (minElement == partitions_[p].end()) ? -1 : minElement->heur;
    } else {
        auto it = boost::find_if(partitions_[p],
                [&status](const Partition& partition)
                { return partition.reachable[status.currentPos()]; });
        return (it == partitions_[p].end()) ? -1 : it->heur;
    }
}



