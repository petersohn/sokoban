#ifndef NODE_H_
#define NODE_H_

#include "Status/State.hpp"
#include "MoveDescriptor.hpp"

#include <boost/serialization/shared_ptr.hpp>

#include <memory>
#include <deque>

namespace sokoban {

class Node {
private:
    std::shared_ptr<Node> ancestor_;
    State state_;
    float heur_;
    float experimentalHeur_;
    MoveDescriptor moveDescriptor_;
    int depth_;
    float cost_;
    int time_;
    std::size_t minDistance_ = 0;
public:
    Node(): heur_(0), experimentalHeur_(0),
            moveDescriptor_(Point{0, 0}, Point{0, 0}),
            depth_(0), cost_(0), time_(0) {}
    Node(const State& stones, const MoveDescriptor& moveDescriptor,
            std::shared_ptr<Node> ancestor, float c, float heur,
            float experimentalHeur, int time);

    Node(const Node&) = delete;
    Node& operator=(const Node&) = delete;

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

    std::size_t minDistance() const { return minDistance_; }
    void minDistance(std::size_t value) { minDistance_ = value; }

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int /*version*/) {
        ar & ancestor_;
        ar & state_;
        ar & heur_;
        ar & experimentalHeur_;
        ar & moveDescriptor_;
        ar & depth_;
        ar & cost_;
        ar & time_;
        ar & minDistance_;
    }
};

std::deque<std::shared_ptr<Node>> pathToBase(std::shared_ptr<Node> node, std::shared_ptr<Node> base);

inline std::deque<std::shared_ptr<Node>> pathToRoot(std::shared_ptr<Node> node) {
    return pathToBase(node, std::shared_ptr<Node>());
}

} // namespace sokoban

#endif /* NODE_H_ */
