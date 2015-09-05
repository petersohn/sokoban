#ifndef SRC_EXTENDEDMOVABLECHECKER_HPP
#define SRC_EXTENDEDMOVABLECHECKER_HPP

#include "Checker/Checker.hpp"

#include <boost/serialization/base_object.hpp>

#include <memory>

class Status;
class HeurCalculator;
class Point;

class ExtendedMovableChecker: public Checker {
    std::shared_ptr<HeurCalculator> calculator_;
public:
    ExtendedMovableChecker(std::shared_ptr<HeurCalculator> calculator = {}):
        calculator_(std::move(calculator))
    {}
    bool check(const Status& status, Point p) const override;
    const char* errorMessage() const override;

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int /*version*/)
    {
        ar & boost::serialization::base_object<Checker>(*this);
        ar & calculator_;
    }
};

#endif /* SRC_EXTENDEDMOVABLECHECKER_HPP */
