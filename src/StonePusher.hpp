#ifndef STONEPUSHER_H_
#define STONEPUSHER_H_

#include "Expander.hpp"
#include "PrioNodeQueueFwd.hpp"
#include "Checker/ComplexCheckerFwd.hpp"
#include "Checker/ComplexCheckerBase.hpp"

class State;
class Node;
class HeurCalculator;
class NodeFactory;

class StonePusher: public Expander {
    std::shared_ptr<Expander> expander_;
    ComplexNodeChecker nodeChecker_;
    std::shared_ptr<const HeurCalculator> calculator_;
    std::shared_ptr<NodeFactory> nodeFactory_;
public:
    StonePusher(std::shared_ptr<Expander> expander, 
            ComplexNodeChecker nodeChecker,
            std::shared_ptr<const HeurCalculator> calculator, 
            std::shared_ptr<NodeFactory> nodeFactory);
    void expand(const Status& status, std::shared_ptr<Node> base,
            PrioNodeQueue& queue, std::shared_ptr<Dumper> dumper) override;
};



#endif /* STONEPUSHER_H_ */
