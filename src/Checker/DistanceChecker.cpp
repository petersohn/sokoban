#include "Checker/DistanceChecker.hpp"

#include "HeurCalculator/HeurCalculator.hpp"
#include "Status/Status.hpp"
#include "Node.hpp"

#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/algorithm.hpp>

#include <functional>

DistanceChecker::DistanceChecker(std::shared_ptr<HeurCalculator> heurCalculator):
        heurCalculator(std::move(heurCalculator))
{
}

bool DistanceChecker::check(const Status& status, Node& node)
{
    if (status.state().size() == 0) {
        return true;
    }

    auto distanceFunction = [&status, this](Point p) {
            return heurCalculator->calculateStone(status, p);
        };
    float distance = *boost::min_element(status.state() |
            boost::adaptors::transformed(std::ref(distanceFunction)));

    if (node.ancestor() && distance < node.ancestor()->minDistance()) {
        return false;
    }

    node.minDistance(distance);
    return true;
}

const char* DistanceChecker::errorMessage() const
{
    return "distance check failed";
}


