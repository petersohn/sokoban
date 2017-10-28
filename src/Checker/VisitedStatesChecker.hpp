#ifndef VISITEDSTATESCHECKER_HPP
#define VISITEDSTATESCHECKER_HPP

#include "Checker/NodeChecker.hpp"
#include "Checker/VisitedStates.hpp"
#include "Node.hpp"

#include <boost/serialization/base_object.hpp>

#include <memory>

namespace sokoban {

class VisitedStatesChecker: public NodeChecker {
    VisitedStates visitedStates;
public:
    VisitedStatesChecker(VisitedStates visitedStates = {}):
            visitedStates(std::move(visitedStates)) {
    }

    bool check(const Status& status, Node& node) override
    {
        VisitedStateInput input(status, node.costFgv());
        return visitedStates.checkAndPush(input);
    }

    const char* errorMessage() const override
    {
        return "already visited";
    }

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int /*version*/)
    {
        ar & boost::serialization::base_object<NodeChecker>(*this);
        ar & visitedStates;
    }
};

} // namespace sokoban

#endif // VISITEDSTATESCHECKER_HPP
