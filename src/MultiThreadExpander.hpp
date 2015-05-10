#ifndef MULTITHREADEXPANDER_HPP
#define MULTITHREADEXPANDER_HPP

#include "Expander.hpp"
#include "ComplexCheckerFwd.hpp"
#include "ComplexCheckerBase.hpp"
#include "MultiThreadWorker.hpp"
#include "Mutexes.hpp"

#include "util/ThreadPool.hpp"

class HeurCalculator;
class NodeFactory;

class MultiThreadExpander: public Expander {
    class InternalExpander;

    std::shared_ptr<const HeurCalculator> calculator_;
    ComplexChecker checker_;
    ComplexNodeChecker nodeChecker_;
    std::shared_ptr<NodeFactory> nodeFactory_;
    util::ThreadPool threadPool_;
    MultiThreadWorker worker_;

    int maxDepth_;
    std::size_t* expandedNodes_;
public:
    MultiThreadExpander(std::shared_ptr<const HeurCalculator> calculator,
            ComplexChecker checker, ComplexNodeChecker nodeChecker,
            std::shared_ptr<NodeFactory> nodeFactory,
            std::size_t* expandedNodes,
            std::size_t numThreads);
    ~MultiThreadExpander();
    void expand(const Status& status, std::shared_ptr<Node> base,
            PrioNodeQueue& queue, std::shared_ptr<Dumper> dumper) override;
};



#endif // MULTITHREADEXPANDER_HPP
