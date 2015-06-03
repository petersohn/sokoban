#ifndef CHECKER_DISTANCECHECKER_HPP
#define CHECKER_DISTANCECHECKER_HPP

#include "Checker/NodeChecker.hpp"

#include <memory>

class HeurCalculator;

class DistanceChecker: public NodeChecker {
public:
    DistanceChecker(std::shared_ptr<HeurCalculator> heurCalculator);
    bool check(const Status& status, Node& node) override;
    const char* errorMessage() const override;

private:
    std::shared_ptr<HeurCalculator> heurCalculator;
};

#endif // CHECKER_DISTANCECHECKER_HPP