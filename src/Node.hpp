#ifndef NODE_H_
#define NODE_H_

#include <memory>
#include <boost/noncopyable.hpp>
#include <deque>
#include "Status/State.hpp"
#include "MoveDescriptor.hpp"

class Node: public boost::noncopyable {
private:
	std::shared_ptr<Node> ancestor_;
	State state_;
	float heur_;
	float experimentalHeur_;
	MoveDescriptor moveDescriptor_;
	int depth_;
	float cost_;
	int time_;
public:
//	Node();
	Node(const State& stones, const MoveDescriptor& moveDescriptor, std::shared_ptr<Node> ancestor,
			float c, float heur, float experimentalHeur, int time);
	float heur() const { return heur_; }
	bool operator==(const Node& other) const { return state_ == other.state_; }
	const std::shared_ptr<Node>& ancestor() const { return ancestor_; }
	const State& state() const { return state_; }
	int depth() const { return depth_; }
	float cost() const { return cost_; }
	float costFgv() const { return cost_ + heur_; }
	Point from() const { return moveDescriptor_.from_; }
	Point  d() const { return moveDescriptor_.d_; }
	const MoveDescriptor& moveDescriptor() const { return moveDescriptor_; }
	int time() const { return time_; }
	float experimtntalHeur() const { return experimentalHeur_; }
	float experimtntalCostFgv() const { return cost_ + experimentalHeur_; }
};

std::deque<std::shared_ptr<Node>> pathToBase(std::shared_ptr<Node> node, std::shared_ptr<Node> base);

inline std::deque<std::shared_ptr<Node>> pathToRoot(std::shared_ptr<Node> node) {
	return pathToBase(node, std::shared_ptr<Node>());
}

#endif /* NODE_H_ */
