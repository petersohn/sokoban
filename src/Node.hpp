#ifndef NODE_H_
#define NODE_H_

#include <memory>
#include <boost/noncopyable.hpp>
#include <deque>
#include "Status/State.hpp"
#include "HeurCalculator.hpp"
#include "MoveDescriptor.hpp"

class Node: public boost::noncopyable {
public:
	typedef std::shared_ptr<Node> Ptr;
private:
	Ptr ancestor_;
	State state_;
	int heur_;
	int experimentalHeur_;
	MoveDescriptor moveDescriptor_;
	int depth_;
	int cost_;
	int time_;
public:
//	Node();
	Node(const State &stones, const MoveDescriptor& moveDescriptor, Ptr ancestor,
			int c, int heur, int experimentalHeur, int time);
	int heur() const { return heur_; }
	bool operator==(const Node &other) const { return state_ == other.state_; }
	const Ptr& ancestor() const { return ancestor_; }
	const State &state() const { return state_; }
	int depth() const { return depth_; }
	int cost() const { return cost_; }
	int costFgv() const { return cost_ + heur_; }
	Point from() const { return moveDescriptor_.from_; }
	Point  d() const { return moveDescriptor_.d_; }
	const MoveDescriptor& moveDescriptor() const { return moveDescriptor_; }
	int time() const { return time_; }
	int experimtntalHeur() const { return experimentalHeur_; }
	int experimtntalCostFgv() const { return cost_ + experimentalHeur_; }
};

std::deque<Node::Ptr> pathToBase(Node::Ptr node, Node::Ptr base);

inline std::deque<Node::Ptr> pathToRoot(Node::Ptr node) {
	return pathToBase(node, Node::Ptr());
}

#endif /* NODE_H_ */
