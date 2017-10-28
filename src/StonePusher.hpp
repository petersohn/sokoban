#ifndef STONEPUSHER_H_
#define STONEPUSHER_H_

#include "Checker/ComplexCheckerBase.hpp"
#include "Checker/ComplexCheckerFwd.hpp"

#include "Expander.hpp"
#include "PrioNodeQueueFwd.hpp"

#include <memory>

namespace sokoban {

class HeurCalculator;
class Node;
class NodeFactory;
class State;

class StonePusher: public Expander {
    std::unique_ptr<Expander> expander_;
    ComplexNodeChecker nodeChecker_;
    std::shared_ptr<const HeurCalculator> calculator_;
    std::shared_ptr<NodeFactory> nodeFactory_;
public:
    StonePusher(std::unique_ptr<Expander> expander,
            ComplexNodeChecker nodeChecker,
            std::shared_ptr<const HeurCalculator> calculator,
            std::shared_ptr<NodeFactory> nodeFactory);
    void expand(const Status& status, std::shared_ptr<Node> base,
            PrioNodeQueue& queue, Dumper* dumper) override;
};

} // namespace sokoban

#endif /* STONEPUSHER_H_ */
