#ifndef DECISIONTREE_H_
#define DECISIONTREE_H_

#include "DecisionTree/NodeBuilder.hpp"

namespace decisionTree {

template <typename Key, typename T, typename PointList>
Node<Key, T> buildNode(
        const typename NodeTypes<Key, T>::ValueList& valueList,
        const PointList& pointList,
        boost::optional<ComplexChecker> checker,
        int maxDepth,
        int numThreads
        )
{
    std::cerr << "Building decision tree\n";
    return detail::NodeBuilder(maxDepth, std::move(checker), numThreads).buildNode<Key, T>(
            valueList,
            pointList);
} // buildNode


} // namespace decisionTree



#endif /* DECISIONTREE_H_ */
