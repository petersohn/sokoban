#ifndef HEURCALCULATOR_H_
#define HEURCALCULATOR_H_

#include <memory>
#include <boost/noncopyable.hpp>

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
            const std::shared_ptr<Node>& ancestor = std::shared_ptr<Node>()) const = 0;
};

#endif /* HEURCALCULATOR_H_ */