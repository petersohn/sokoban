#ifndef DECISIONTREE_H_
#define DECISIONTREE_H_

#include <memory>
#include <boost/range/algorithm.hpp>
#include <boost/range/adaptors.hpp>
#include <vector>
#include <cstdlib>
#include <iostream>


namespace decisionTree {


template <class Key, class T>
class Node {
public:
	typedef std::pair<Key, T> Value;
	virtual const Value* get(const Key& key) = 0;
	virtual ~Node() {}
};

namespace detail {

	template <class Key, class T>
	class LeafNode: public Node<Key, T> {
		typedef typename Node<Key, T>::Value Value;
		Value value_;
	public:
		LeafNode(const Value& value):
			value_(value) {}
		virtual const Value* get(const Key&)
		{
//			std::cout << "leafNode: (" << value_.first << ", " << value_.second << ")" << std::endl;
			return &value_;
		}
	};

	template <class Key, class T>
	class EmptyLeafNode: public Node<Key, T> {
		typedef typename Node<Key, T>::Value Value;
	public:
		virtual const Value* get(const Key&) { return 0; }
	};


	template <class Key, class T, class Functor>
	struct DecisionNode: public Node<Key, T> {
		typedef typename Node<Key, T>::Value Value;
		Functor functor_;
		std::unique_ptr<Node<Key, T>> falseChild_;
		std::unique_ptr<Node<Key, T>> trueChild_;
	public:
		template <class ValueList, class FunctorList, class SplittingAlgorithm>
		DecisionNode(
				const ValueList& valueList,
				const FunctorList& functorList,
				const SplittingAlgorithm& splittingAlgoritm)
		{
			typedef typename ValueList::value_type Value;
			typedef std::vector<const Value*> Values;

			typename FunctorList::const_iterator bestSplit = splittingAlgoritm(valueList, functorList);
			functor_ = *bestSplit;
			Values trueValues;
			Values falseValues;
			for (const Value& value: valueList) {
				if (functor_(value.first)) {
					trueValues.push_back(&value);
				} else {
					falseValues.push_back(&value);
				}
			}

			std::vector<Functor> newFunctorList;
			newFunctorList.reserve(functorList.size() - 1);
			std::copy(functorList.begin(), bestSplit, std::back_inserter(newFunctorList));
			std::copy(++bestSplit, functorList.end(), std::back_inserter(newFunctorList));
			falseChild_ = buildNode(falseValues | boost::adaptors::indirected,
					newFunctorList, splittingAlgoritm);
			trueChild_ = buildNode(trueValues | boost::adaptors::indirected,
					newFunctorList, splittingAlgoritm);
		}
		virtual const Value* get(const Key& key)
		{
//			std::cout << "decision node: (" << key << "->" << arg_ << "): ";
			if (functor_(key)) {
//				std::cout << "true" << std::endl;
				return trueChild_->get(key);
			} else {
//				std::cout << "false" << std::endl;
				return falseChild_->get(key);
			}
		}
	};

}

template <class ValueList, class FunctorList, class SplittingAlgorithm>
std::unique_ptr<Node<typename ValueList::value_type::first_type, typename ValueList::value_type::second_type>>
buildNode(
		const ValueList& valueList,
		const FunctorList& functorList,
		const SplittingAlgorithm& splittingAlgoritm)
{
	typedef typename ValueList::value_type::first_type Key;
	typedef typename ValueList::value_type::second_type T;
	typedef typename FunctorList::value_type Functor;

	if (valueList.size() == 1) {
		return std::unique_ptr<Node<Key, T>>(new detail::LeafNode<Key, T>(*std::begin(valueList)));
	} else
	if (valueList.size() == 0) {
		return std::unique_ptr<Node<Key, T>>(new detail::EmptyLeafNode<Key, T>());
	} else {
		return std::unique_ptr<Node<Key, T>>(
				new detail::DecisionNode<Key, T, Functor>(
						valueList, functorList, splittingAlgoritm));
	}
}


} // namespace decisionTree



#endif /* DECISIONTREE_H_ */
