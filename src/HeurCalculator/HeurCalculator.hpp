#ifndef HEURCALCULATOR_HEURCALCULATOR_HPP
#define HEURCALCULATOR_HEURCALCULATOR_HPP

#include <memory>
#include <boost/noncopyable.hpp>

namespace sokoban {

class Point;
class Status;
class Node;
struct MoveDescriptor;

class HeurCalculator: public boost::noncopyable {
public:
    virtual ~HeurCalculator() {}
    virtual float calculateStone(const Status& status, Point  p) const = 0;
    virtual float calculateStatus(
            const Status& status,
            const MoveDescriptor* moveDescriptor = nullptr,
            const std::shared_ptr<Node>& ancestor =
                    std::shared_ptr<Node>()) const = 0;

    template <typename Archive>
    void serialize(Archive& /*ar*/, const unsigned int /*version*/)
    {
    }
};

} // namespace sokoban

#endif // HEURCALCULATOR_HEURCALCULATOR_HPP
