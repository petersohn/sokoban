#ifndef DECISIONTREE_H_
#define DECISIONTREE_H_

#include <memory>
#include <boost/range/algorithm.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/format.hpp>
#include <vector>
#include <cstdlib>
#include "AnnotatedFunction.h"
#include "Dumper/IndentedOutput.h"


namespace decisionTree {


template <class Key, class T>
class Node {
public:
	typedef std::pair<Key, T> Value;
	typedef std::shared_ptr<Value> ValuePtr;
	typedef std::vector<ValuePtr> ValueList;
	virtual const ValueList& get(const Key& key) = 0;
	virtual ~Node() {}
};

namespace detail {

	const int indentLevel = 2;

	template <class Key, class T>
	class LeafNode: public Node<Key, T> {
		typedef typename Node<Key, T>::Value Value;
		typedef typename Node<Key, T>::ValueList ValueList;
		ValueList value_;
	public:
		LeafNode(const ValueList& value):
			value_(value) {}
		LeafNode(ValueList&& value):
			value_(std::move(value)) {}
		virtual const ValueList& get(const Key&)
		{
			return value_;
		}
	};

	template <class Key, class T, class Functor>
	class DecisionNode: public Node<Key, T> {
		typedef typename Node<Key, T>::Value Value;
		typedef typename Node<Key, T>::ValueList ValueList;
		Functor functor_;

		typedef std::unique_ptr<Node<Key, T>> ChildType;
		ChildType falseChild_;
		ChildType trueChild_;
	public:
		DecisionNode(const Functor& functor,
				ChildType&& falseChild, ChildType&& trueChild):
			functor_(functor),
			falseChild_(std::move(falseChild)),
			trueChild_(std::move(trueChild))
		{}

		virtual const ValueList& get(const Key& key)
		{
			if (functor_(key)) {
				return trueChild_->get(key);
			} else {
				return falseChild_->get(key);
			}
		}
	}; // class DecisionNode



} // namespace detail

template <class Key, class T, class FunctorList, class SplittingAlgorithm>
std::unique_ptr<Node<Key, T>>
buildNode(
		typename Node<Key, T>::ValueList&& valueList,
		const FunctorList& functorList,
		const SplittingAlgorithm& splittingAlgoritm,
		int depthToGo
		)
	{
		typedef typename Node<Key, T>::ValuePtr ValuePtr;
		typedef typename Node<Key, T>::ValueList ValueList;
		typedef typename FunctorList::value_type Functor;

		if (valueList.size() == 0 || depthToGo == 0) {
			return std::unique_ptr<Node<Key, T>>(
					new detail::LeafNode<Key, T>(std::move(valueList)));
		} else {
			typename FunctorList::const_iterator bestSplit =
					splittingAlgoritm(valueList, functorList);
			assert(bestSplit != std::end(functorList));
			Functor functor = *bestSplit;
			ValueList falseValues;
			boost::remove_copy_if(valueList,
					std::back_inserter(falseValues),
					[&functor](const ValuePtr& value)
					{ return functor(value->first); });


			std::vector<Functor> newFunctorList;
			newFunctorList.reserve(functorList.size() - 1);
			std::copy(functorList.begin(), bestSplit,
					std::back_inserter(newFunctorList));
			std::copy(++bestSplit, functorList.end(),
					std::back_inserter(newFunctorList));
			if (falseValues.size() == valueList.size()) {
				return std::unique_ptr<Node<Key, T>>(
					new detail::LeafNode<Key, T>(std::move(valueList)));
			}
			return std::unique_ptr<Node<Key, T>>(
					new detail::DecisionNode<Key, T, Functor>(
							functor,
							buildNode<Key, T>(std::move(falseValues),
									newFunctorList,
									splittingAlgoritm,
									depthToGo - 1),
							buildNode<Key, T>(std::move(valueList),
									newFunctorList,
									splittingAlgoritm,
									depthToGo - 1)
					));
		}
	} // buildNode


} // namespace decisionTree



#endif /* DECISIONTREE_H_ */
