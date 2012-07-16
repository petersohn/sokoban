#ifndef DECISIONTREE_H_
#define DECISIONTREE_H_

#include <memory>
#include <boost/range/algorithm.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/format.hpp>
#include <vector>
#include <cstdlib>
#include <iostream>
#include "AnnotatedFunction.h"
#include "Dumper/IndentedOutput.h"


namespace decisionTree {


template <class Key, class T>
class Node {
public:
	typedef std::pair<Key, T> Value;
	virtual const Value* get(const Key& key) = 0;
	virtual ~Node() {}
};

namespace detail {

	const int indentLevel = 2;

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
				const SplittingAlgorithm& splittingAlgoritm,
				int level)
		{
			typedef typename ValueList::value_type Value;
			typedef std::vector<const Value*> Values;

			typename FunctorList::const_iterator bestSplit = splittingAlgoritm(valueList, functorList);
			assert(bestSplit != std::end(functorList));
			functor_ = *bestSplit;
			indentedOutput(std::cerr, functionName(functor_), level);
			Values trueValues;
			Values falseValues;
			for (const Value& value: valueList) {
				if (functor_(value.first)) {
//					indentedOutput(std::cerr, "True", level);
					trueValues.push_back(&value);
				} else {
//					indentedOutput(std::cerr, "False", level);
					falseValues.push_back(&value);
				}
//				indentedOutput(std::cerr, value.first, level);
			}

			std::vector<Functor> newFunctorList;
			newFunctorList.reserve(functorList.size() - 1);
			std::copy(functorList.begin(), bestSplit, std::back_inserter(newFunctorList));
			std::copy(++bestSplit, functorList.end(), std::back_inserter(newFunctorList));
			boost::format branchOutputFormat("%s branch (%d)");
			indentedOutput(std::cerr, (boost::format(branchOutputFormat) % "False" % falseValues.size()).str(), level);
			falseChild_ = buildNode(falseValues | boost::adaptors::indirected,
					newFunctorList, splittingAlgoritm, level + indentLevel);
			indentedOutput(std::cerr, (boost::format(branchOutputFormat) % "True" % trueValues.size()).str(), level);
			trueChild_ = buildNode(trueValues | boost::adaptors::indirected,
					newFunctorList, splittingAlgoritm, level + indentLevel);
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
		const SplittingAlgorithm& splittingAlgoritm,
		int level = 0
		)
{
	typedef typename ValueList::value_type::first_type Key;
	typedef typename ValueList::value_type::second_type T;
	typedef typename FunctorList::value_type Functor;
	assert(valueList.size() != 0);
	if (valueList.size() == 1) {
		const auto& value = *std::begin(valueList);
		indentedOutput(std::cerr, "Final value", level);
		indentedOutput(std::cerr, value.first, level);
		indentedOutput(std::cerr, value.second, level);
		return std::unique_ptr<Node<Key, T>>(new detail::LeafNode<Key, T>(value));
	} else
	if (valueList.size() == 0) {
		indentedOutput(std::cerr, "No value", level);
		return std::unique_ptr<Node<Key, T>>(new detail::EmptyLeafNode<Key, T>());
	} else {
		indentedOutput(std::cerr, "Decision point", level);
		return std::unique_ptr<Node<Key, T>>(
				new detail::DecisionNode<Key, T, Functor>(
						valueList, functorList, splittingAlgoritm, level));
	}
}


} // namespace decisionTree



#endif /* DECISIONTREE_H_ */
