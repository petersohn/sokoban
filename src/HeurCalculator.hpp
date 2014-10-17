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
	typedef std::shared_ptr<const HeurCalculator> Ptr;
	virtual ~HeurCalculator() {}
	virtual int calculateStone(const Status& status, Point  p) const = 0;
	virtual int calculateStatus(
			const Status& status,
			const MoveDescriptor* moveDescriptor = nullptr,
			const std::shared_ptr<Node>& ancestor = std::shared_ptr<Node>()) const = 0;
};

#endif /* HEURCALCULATOR_H_ */
