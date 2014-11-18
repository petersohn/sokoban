#ifndef DECISIONTREE_H_
#define DECISIONTREE_H_

#include "DecisionTree/NodeBuilder.hpp"

namespace decisionTree {

template <class Key, class T, class PointList>
std::unique_ptr<Node<Key, T>>
buildNode(
		const typename Node<Key, T>::ValueList& valueList,
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
