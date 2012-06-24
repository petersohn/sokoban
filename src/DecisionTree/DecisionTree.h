#ifndef DECISIONTREE_H_
#define DECISIONTREE_H_

#include <memory>
#include <boost/range/algorithm.hpp>
#include <boost/range/adaptors.hpp>
#include <vector>
#include <cstdlib>
#include <iostream>


namespace decisionTree {


template <class Arg>
struct SplittingValue {
	Arg arg_;
	int trueNum_;
	int falseNum_;
	SplittingValue(
			const Arg& arg,
			int trueNum,
			int falseNum):
		arg_(arg),
		trueNum_(trueNum),
		falseNum_(falseNum) {}
};


template <class Key, class T>
class Node {
public:
	typedef std::pair<Key, T> Value;
	virtual const Value* get(const Key& key) = 0;
	virtual ~Node() {}
};

template <class Functor, class ValueList, class ArgList>
std::unique_ptr<Node<typename ValueList::value_type::first_type, typename ValueList::value_type::second_type>>
buildNode(
		Functor& functor,
		const ValueList& valueList,
		const ArgList& argList);

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
			std::cout << "leafNode: (" << value_.first << ", " << value_.second << ")" << std::endl;
			return &value_;
		}
	};

	template <class Key, class T>
	class EmptyLeafNode: public Node<Key, T> {
		typedef typename Node<Key, T>::Value Value;
	public:
		virtual const Value* get(const Key&) { return 0; }
	};


	template <class Key, class T, class Arg, class Functor>
	struct DecisionNode: public Node<Key, T> {
		typedef typename Node<Key, T>::Value Value;
		Functor& functor_;
		Arg arg_;
		std::unique_ptr<Node<Key, T>> falseChild_;
		std::unique_ptr<Node<Key, T>> trueChild_;
	public:
		template <class ValueList, class ArgList, class SplittingAlgorithm>
		DecisionNode(
				Functor& functor,
				const ValueList& valueList,
				const ArgList& argList,
				const SplittingAlgorithm& splittingAlgoritm):
			functor_(functor)
		{
			typedef typename ValueList::value_type Value;
			typedef std::vector<const Value*> Values;

			SplittingValue<Arg> bestSplit = splittingAlgoritm(functor, valueList, argList);
			Values trueValues;
			trueValues.reserve(bestSplit.trueNum_);
			Values falseValues;
			falseValues.reserve(bestSplit.falseNum_);
			for (const Value& value: valueList) {
				if (functor(value.first, bestSplit.arg_)) {
					trueValues.push_back(&value);
				} else {
					falseValues.push_back(&value);
				}
			}

			std::vector<Arg> newArgList;
			newArgList.reserve(argList.size() - 1);
			boost::remove_copy(argList, std::back_inserter(newArgList), bestSplit.arg_);
			falseChild_ = buildNode(functor, falseValues | boost::adaptors::indirected,
					newArgList, splittingAlgoritm);
			trueChild_ = buildNode(functor, trueValues | boost::adaptors::indirected,
					newArgList, splittingAlgoritm);
			arg_ = bestSplit.arg_;
		}
		virtual const Value* get(const Key& key)
		{
			std::cout << "decision node: (" << key << "->" << arg_ << "): ";
			if (functor_(key, arg_)) {
				std::cout << "true" << std::endl;
				return trueChild_->get(key);
			} else {
				std::cout << "false" << std::endl;
				return falseChild_->get(key);
			}
		}
	};

}

template <class Functor, class ValueList, class ArgList, class SplittingAlgorithm>
std::unique_ptr<Node<typename ValueList::value_type::first_type, typename ValueList::value_type::second_type>>
buildNode(
		Functor& functor,
		const ValueList& valueList,
		const ArgList& argList,
		const SplittingAlgorithm& splittingAlgoritm)
{
	typedef typename ValueList::value_type::first_type Key;
	typedef typename ValueList::value_type::second_type T;
	if (valueList.size() == 1) {
		return std::unique_ptr<Node<Key, T>>(new detail::LeafNode<Key, T>(*std::begin(valueList)));
	} else
	if (valueList.size() == 0) {
		return std::unique_ptr<Node<Key, T>>(new detail::EmptyLeafNode<Key, T>());
	} else {
		return std::unique_ptr<Node<Key, T>>(
				new detail::DecisionNode<Key, T, typename ArgList::value_type, Functor>(
						functor, valueList, argList, splittingAlgoritm));
	}
}


} // namespace decisionTree



#endif /* DECISIONTREE_H_ */
