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

bool DistanceChecker::check(const Status& status, const Node& /*node*/)
{
    //std::cerr << "stones=" << status.state().size();
    if (status.state().size() == 0) {
        return true;
    }

    auto distanceFunction = [&status, this](Point p) {
            return heurCalculator->calculateStone(status, p);
        };
    float distance = *boost::min_element(status.state() |
            boost::adaptors::transformed(std::ref(distanceFunction)));

    if (distance < currentDistance) {
        //std::cerr << "Reject\n";
        return false;
    }

    //std::cerr << "Acccept: new distance = " << distance << "\n";
    currentDistance = distance;
    return true;
}

const char* DistanceChecker::errorMessage() const
{
    return "distance check failed";
}


