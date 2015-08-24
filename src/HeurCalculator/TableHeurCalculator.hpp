#ifndef TABLEHEURCALCULATOR_H_
#define TABLEHEURCALCULATOR_H_

#include "HeurCalculator/HeurCalculator.hpp"
#include "Status/Status.hpp"
#include "Status/Table.hpp"
#include "Array.hpp"
#include "Mutexes.hpp"

template <typename StoneCalculator>
class TableHeurCalculator: public HeurCalculator {
    StoneCalculator calculator_;
    float heurMultiplier_;
public:
    TableHeurCalculator():
            calculator_{}, heurMultiplier_{1.0}
    {}

    TableHeurCalculator(StoneCalculator calculator, float heurMultiplier):
        calculator_(std::move(calculator)),
        heurMultiplier_(heurMultiplier)
    {}

    float calculateStatus(
            const Status& status,
            const MoveDescriptor*,
            const std::shared_ptr<Node>&) const override
    {
        float result = 0;
        for (auto stone: status.state()) {
            float val = calculator_(status, stone) * heurMultiplier_;
            if (val < 0)
                return -1;
            result += val;
        }
        return result;
    }

    float calculateStone(const Status& status, Point p) const override
    {
        return calculator_(status, p) * heurMultiplier_;
    }

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int /*version*/)
    {
        ar & boost::serialization::base_object<HeurCalculator>(*this);
        ar & calculator_;
        ar & heurMultiplier_;
    }
};

#endif /* TABLEHEURCALCULATOR_H_ */
