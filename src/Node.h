#ifndef NODE_H_
#define NODE_H_

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <deque>
#include "State.h"
#include "HeurCalculator.h"

class Node: public boost::noncopyable {
public:
	typedef boost::shared_ptr<Node> Ptr;
private:
	Ptr ansector_;
	State state_;
	int heur_;
	Point from_;
	Point d_;
	int depth_;
	int cost_;
	int time_;
public:
	Node();
	Node(const State &stones, const Point &from, const Point &d, Ptr ans,
			int c, int heur, int time);
	int heur() const { return heur_; }
	bool operator==(const Node &other) const { return state_ == other.state_; }
	const Ptr& ansector() const { return ansector_; }
	const State &state() const { return state_; }
	int depth() const { return depth_; }
	int cost() const { return cost_; }
	int costFgv() const { return cost_ + heur_; }
	const Point &from() const { return from_; }
	const Point& d() const { return d_; }
	int time() const { return time_; }
	bool operator<(const Node &other) const
	{
		return (costFgv() < other.costFgv() ? false : (costFgv() > other.costFgv() ? true :
			(other.depth() - depth()) > 0));
	}

};


std::deque<Node::Ptr> pathToBase(Node::Ptr node, Node::Ptr base);

inline std::deque<Node::Ptr> pathToRoot(Node::Ptr node) {
	return pathToBase(node, Node::Ptr());
}

#endif /* NODE_H_ */
