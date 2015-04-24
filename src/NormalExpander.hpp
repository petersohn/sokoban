#ifndef NORMALEXPANDER_H_
#define NORMALEXPANDER_H_

#include "Expander.hpp"
#include "ComplexCheckerFwd.hpp"
#include "ComplexCheckerBase.hpp"
#include "Mutexes.hpp"

class VisitedStates;
class HeurCalculator;
class NodeFactory;

class NormalExpander: public Expander {
    friend class InternalExpander;

    std::shared_ptr<VisitedStates> visitedStates_;
    std::shared_ptr<const HeurCalculator> calculator_;
    ComplexChecker checker_;
    std::shared_ptr<NodeFactory> nodeFactory_;
    int maxDepth_;
    std::size_t* expandedNodes_;
    std::size_t expandedNodeLimit_;
public:
    NormalExpander(std::shared_ptr<VisitedStates> vs,
            std::shared_ptr<const HeurCalculator> calculator,
            ComplexChecker ch, std::shared_ptr<NodeFactory> nodeFactory,
            std::size_t* expandedNodes = nullptr,
            std::size_t expandedNodeLimit = 0);
    ~NormalExpander();
    void expand(const Status& status, std::shared_ptr<Node> base,
            PrioNodeQueue& queue, std::shared_ptr<Dumper> dumper) override;
};

#endif /* NORMALEXPANDER_H_ */
