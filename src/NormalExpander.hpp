#ifndef NORMALEXPANDER_H_
#define NORMALEXPANDER_H_

#include "Checker/ComplexCheckerBase.hpp"
#include "Checker/ComplexCheckerFwd.hpp"
#include "Expander.hpp"
#include "Mutexes.hpp"

namespace sokoban {

class HeurCalculator;
class NodeFactory;

class NormalExpander: public Expander {
    class InternalExpander;

    std::shared_ptr<const HeurCalculator> calculator_;
    ComplexChecker checker_;
    ComplexNodeChecker nodeChecker_;
    std::shared_ptr<NodeFactory> nodeFactory_;
    int maxDepth_;
    std::size_t* expandedNodes_;
public:
    NormalExpander(std::shared_ptr<const HeurCalculator> calculator,
            ComplexChecker checker, ComplexNodeChecker nodeChecker,
            std::shared_ptr<NodeFactory> nodeFactory,
            std::size_t* expandedNodes = nullptr);
    ~NormalExpander();
    void expand(const Status& status, std::shared_ptr<Node> base,
            PrioNodeQueue& queue, Dumper* dumper) override;
};

} // namespace sokoban

#endif /* NORMALEXPANDER_H_ */
