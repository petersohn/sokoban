#ifndef DECISIONTREE_NODE_H_
#define DECISIONTREE_NODE_H_

#include <Status/Point.hpp>

#include <boost/variant.hpp>
#include <boost/variant/recursive_variant.hpp>

#include <memory>
#include <vector>

namespace decisionTree {

template <class Status, class T>
class NodeTypes {
public:
    typedef std::pair<Status, T> Value;
    typedef std::shared_ptr<Value> ValuePtr;
    typedef std::shared_ptr<const Value> ValueConstPtr;
    typedef std::vector<ValuePtr> ValueList;
};

template <class Status, class T>
class NodeVisitor: public boost::static_visitor<
       const typename NodeTypes<Status, T>::ValueList&> {
    const Status& status;
public:
    NodeVisitor(const Status& status): status(status) {}

    template <typename Node>
    decltype(auto) operator()(const Node& node) const {
        return node.get(status);
    }
};

template <class Status, class T>
class LeafNode {
    typedef typename NodeTypes<Status, T>::Value Value;
    typedef typename NodeTypes<Status, T>::ValueList ValueList;
    ValueList value_;
public:
    LeafNode() = default;
    LeafNode(const ValueList& value):
        value_(value) {}
    LeafNode(ValueList&& value):
        value_(std::move(value)) {}
    const ValueList& get(const Status&) const
    {
        return value_;
    }
};

template <class Status, class T, class Node>
class DecisionNode {
    typedef typename NodeTypes<Status, T>::Value Value;
    typedef typename NodeTypes<Status, T>::ValueList ValueList;
    Point point_;

    using ChildType = Node;
    ChildType falseChild_;
    ChildType trueChild_;
public:
    DecisionNode(Point  point):
        point_(point)
    {}

    DecisionNode(Point  point,
            ChildType&& falseChild, ChildType&& trueChild):
        point_(point),
        falseChild_(std::move(falseChild)),
        trueChild_(std::move(trueChild))
    {}

    const ValueList& get(const Status& status) const
    {
        NodeVisitor<Status, T> visitor(status);
        if (isStone(status, point_)) {
            return boost::apply_visitor(visitor, trueChild_);
        } else {
            return boost::apply_visitor(visitor, falseChild_);
        }
    }

    ChildType& falseChild() { return falseChild_; }
    ChildType& trueChild() { return trueChild_; }
}; // class DecisionNode

template <class Status, class T>
using Node = typename boost::make_recursive_variant<
                LeafNode<Status, T>,
                DecisionNode<Status, T, boost::recursive_variant_>
        >::type;

}


#endif /* DECISIONTREE_NODE_H_ */
