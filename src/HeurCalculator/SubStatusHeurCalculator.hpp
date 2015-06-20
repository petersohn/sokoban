#ifndef SRC_SUBSTATUSHEURCALCULATOR_HPP
#define SRC_SUBSTATUSHEURCALCULATOR_HPP

#include "HeurCalculator/HeurCalculator.hpp"
#include "Status/PseudoStatus.hpp"
#include "Node.hpp"

#include <boost/serialization/base_object.hpp>

template <typename NextFactory>
class SubStatusHeurCalculator: public HeurCalculator {
    float heurMultiplier_;
    std::shared_ptr<const HeurCalculator> baseCalculator_;
    NextFactory nextFactory_;
public:
    SubStatusHeurCalculator() : heurMultiplier_(0.0) {
    }

    SubStatusHeurCalculator(
            float heurMultiplier,
            const std::shared_ptr<const HeurCalculator>& baseCalculator,
            NextFactory nextFactory = NextFactory{}):
                heurMultiplier_(heurMultiplier),
                baseCalculator_(std::move(baseCalculator)),
                nextFactory_(std::move(nextFactory))
    {
    }

    void setBaseCalculator(const std::shared_ptr<const HeurCalculator>& value)
    {
        baseCalculator_ = value;
    }

    void setHeurMultiplier(float value)
    {
        heurMultiplier_ = value;
    }

    float calculateStone(const Status& status, Point p) const override
    {
        return baseCalculator_->calculateStone(status, p) * heurMultiplier_;
    }

    float calculateStatus(
            const Status& status,
            const MoveDescriptor* /*moveDescriptor*/,
            const std::shared_ptr<Node>& ancestor) const override
    {
        PseudoStatus pseudoStatus(status);
        float result = 0;
        auto next = nextFactory_();
        while (const auto& subset = next(pseudoStatus)) {
            if (isSubStatus(subset->first, pseudoStatus)) {
                result += subset->second;
                for (Point  p: subset->first.state()) {
                    pseudoStatus.state().removeStone(p);
                }
            }
        }
        for (Point  p: pseudoStatus.state()) {
            result += baseCalculator_->calculateStone(status, p);
        }
        if (ancestor) {
            result = std::max(result, ancestor->heur() / heurMultiplier_ - 1.0f);
        }
        return result * heurMultiplier_;
    }

    template <typename Ar>
    void serialize(Ar& ar, const unsigned int /*version*/) {
        ar & boost::serialization::base_object<HeurCalculator>(*this);
        ar & nextFactory_;
    }
};

#endif /* SRC_SUBSTATUSHEURCALCULATOR_HPP */
