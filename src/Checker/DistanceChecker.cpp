#include "Checker/DistanceChecker.hpp"

#include "HeurCalculator/HeurCalculator.hpp"
#include "Status/Status.hpp"
#include "Node.hpp"

#include <boost/bind.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/algorithm.hpp>

DistanceChecker::DistanceChecker(std::shared_ptr<HeurCalculator> heurCalculator):
        heurCalculator(std::move(heurCalculator))
{
}

bool DistanceChecker::check(const Status& status, const Node& /*node*/)
{
    float distance = *boost::min_element(status.state() |
            boost::adaptors::transformed(boost::bind(
                    &HeurCalculator::calculateStone, heurCalculator.get(),
                    std::ref(status), _1)));

    if (distance < currentDistance) {
        return false;
    }

    currentDistance = distance;
    return 0;
}

const char* DistanceChecker::errorMessage() const
{
    return "distance check failed";
}


