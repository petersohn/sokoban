#ifndef NORMALEXPANDER_H_
#define NORMALEXPANDER_H_

#include "Expander.hpp"
#include "ComplexCheckerFwd.hpp"
#include "ComplexCheckerBase.hpp"
#include "Mutexes.hpp"

class HeurCalculator;
class NodeFactory;

class NormalExpander: public Expander {
    friend class InternalExpander;

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
            PrioNodeQueue& queue, std::shared_ptr<Dumper> dumper) override;
};

#endif /* NORMALEXPANDER_H_ */
