#ifndef SOLVER_H_
#define SOLVER_H_

#include "Dumper/DumperFactory.hpp"
#include "PrioNodeQueueFwd.hpp"
#include "ExpanderFactory.hpp"
#include <deque>
#include <functional>
#include <memory>

class Status;
class Node;

class NullDumperFactory {
public:
    std::unique_ptr<Dumper> operator()() { return {}; }
};

class Solver {
private:
    QueueFactory queueFactory_;
    ExpanderFactory expanderFactory_;
    DumperFactory dumperFactory_;
public:
    Solver(QueueFactory qf, ExpanderFactory ef,
            DumperFactory df = NullDumperFactory()):
        queueFactory_(qf),
        expanderFactory_(ef),
        dumperFactory_(df)
    {
    }
    std::deque<std::shared_ptr<Node>> solve(Status status) const;
    ~Solver() {}
private:
};

#endif /* SOLVER_H_ */
