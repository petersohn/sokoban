#ifndef MOVABLECHECKER_H_
#define MOVABLECHECKER_H_

#include "Checker/Checker.hpp"
#include "Status/Point.hpp"

#include <boost/serialization/base_object.hpp>

#include <memory>

namespace sokoban {

class Status;
class HeurCalculator;

class MovableChecker: public Checker {
    std::shared_ptr<HeurCalculator> calculator_;
public:
    MovableChecker(std::shared_ptr<HeurCalculator> calculator = {}):
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

} // namespace sokoban

#endif /* MOVABLECHECKER_H_ */
