#ifndef NODE_H_
#define NODE_H_

#include <boost/shared_ptr.hpp>
#include <deque>
#include "State.h"
#include "HeurCalculator.h"

class Node {
public:
	typedef boost::shared_ptr<Node> Ptr;
private:
	Ptr ansector_;
	State state_;
	int heur_;
	int stone_;
	Point d_;
	int depth_;
	int cost_;

	Node();
	Node(const State &stones, int stone, const Point &d, Ptr ans,
			int c, int heur);
	void copy(const Node &other);
	static Ptr createNew(Node *np) {
		Ptr n(np);
		return n;
	}
public:
	static Ptr create() {
		return createNew(new Node());
	}
	static Ptr create(const State &stones, int stone, const Point &d,
			Ptr ans, int c, int heur) {
		return createNew(new Node(stones, stone, d, ans, c, heur));
	}
	Node(const Node &other) {
		copy(other);
	}
	Node &operator=(const Node &other) {
		copy(other);
		return *this;
	}
	int heur() const { return heur_; }
	bool operator==(const Node &other) const { return state_ == other.state_; }
	const Ptr& ansector() const { return ansector_; }
	const State &state() const { return state_; }
	int depth() const { return depth_; }
	int cost() const { return cost_; }
	int costFgv() const { return cost_ + heur_; }
	int stone() const { return stone_; }
	const Point& d() const { return d_; }
	Point currentPos() const { return state_[stone_] - d_; }
	bool operator<(const Node &other) const
	{
		return (costFgv() < other.costFgv() ? false : (costFgv() > other.costFgv() ? true :
			(other.depth() - depth()) > 0));
	}

};
/*
inline bool operator==(const Node::Ptr a, const Node::Ptr b)
{
	if (a.get() == NULL)
		return b.get() == NULL;
	return a.get() == b.get() || *a == *b;
}
*/

struct NodeCompare {
	bool operator()(const Node::Ptr a, const Node::Ptr b)
	{
		if (a.get() == NULL || b.get() == NULL)
			return false;
		return *a < *b;
	}
};

std::deque<Node::Ptr> pathToRoot(Node::Ptr node);



#endif /* NODE_H_ */
