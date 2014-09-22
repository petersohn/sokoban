#ifndef DECISIONTREE_H_
#define DECISIONTREE_H_

#include "DecisionTree/NodeBuilder.hpp"

namespace decisionTree {

template <class Key, class T, class PointList>
std::unique_ptr<Node<Key, T>>
buildNode(
		const typename Node<Key, T>::ValueList& valueList,
		const PointList& pointList,
		const Checker::Ptr& checker,
		int maxDepth,
		int numThreads
		)
{
	std::cerr << "Building decision tree\n";
	return detail::NodeBuilder(maxDepth, checker, numThreads).buildNode<Key, T>(
			valueList,
			pointList);
} // buildNode


} // namespace decisionTree



#endif /* DECISIONTREE_H_ */
