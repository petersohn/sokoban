#ifndef CHECKER_DISTANCECHECKER_HPP
#define CHECKER_DISTANCECHECKER_HPP

#include <boost/serialization/type_info_implementation.hpp>
#include <boost/serialization/shared_ptr.hpp>

#include "Checker/NodeChecker.hpp"

#include <memory>

namespace sokoban {

class HeurCalculator;

class DistanceChecker: public NodeChecker {
public:
    DistanceChecker(std::shared_ptr<HeurCalculator> heurCalculator = {});
    bool check(const Status& status, Node& node) override;
    const char* errorMessage() const override;

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int /*version*/)
    {
        ar & boost::serialization::base_object<NodeChecker>(*this);
        ar & heurCalculator;
    }
private:
    std::shared_ptr<HeurCalculator> heurCalculator;
};

} // namespace sokoban

#endif // CHECKER_DISTANCECHECKER_HPP
