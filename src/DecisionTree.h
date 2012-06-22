#ifndef DECISIONTREE_H_
#define DECISIONTREE_H_

#include <memory>
#include <boost/range/algorithm.hpp>
#include <boost/range/adaptors.hpp>
#include <vector>
#include <cstdlib>


namespace decisionTree {

template <class Key, class T>
class Node {
public:
	virtual const T* get(const Key& key) = 0;
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
		T value_;
	public:
		LeafNode(const T& value):
			value_(value) {}
		virtual const T* get(const Key&) { return &value_; }
	};

	template <class Key, class T>
	class EmptyLeafNode: public Node<Key, T> {
	public:
		virtual const T* get(const Key&) { return 0; }
	};

	template <class Arg>
	struct SplittingValue {
		const Arg& arg_;
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

	template <class Arg>
	static bool operator<(const SplittingValue<Arg>& left, const SplittingValue<Arg>& right)
	{
		return std::abs(left.trueNum - left.falseNum) <
				std::abs(right.trueNum - right.falseNum);
	}

	template <class ValueList, class Arg, class Functor>
	SplittingValue<Arg> calculateSplittingValue(
			Functor& functor,
			const ValueList& valueList,
			const Arg& arg)
	{
		int trueNum = 0;
		int falseNum = 0;
		for (const auto& value: valueList) {
			if (functor(value.first, arg)) {
				++trueNum;
			} else {
				--falseNum;
			}
			return SplittingValue<Arg>(arg, trueNum, falseNum);
		}
	}

	template <class Key, class T, class Arg, class Functor>
	struct DecisionNode: public Node<Key, T> {
		Functor& functor_;
		Arg arg_;
		std::unique_ptr<Node<Key, T>> falseChild_;
		std::unique_ptr<Node<Key, T>> trueChild_;
	public:
		template <class ValueList, class ArgList>
		DecisionNode(
				Functor& functor,
				const ValueList& valueList,
				const ArgList& argList):
			functor_(functor)
		{
			typedef typename ValueList::value_type Value;
			typedef std::vector<const Value*> Values;

			std::vector<SplittingValue<Arg>> splittingValues;
			splittingValues.reserve(valueList.size());
			boost::transform(
					argList, std::back_inserter(splittingValues),
					[&functor, &valueList](const Arg& arg)
					{
						return calculateSplittingValue(functor, valueList, arg);
					});
			const SplittingValue<Arg>& bestSplit = boost::min_element(splittingValues);
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
			falseChild_ = buildNode(functor, falseValues, newArgList);
			trueChild_ = buildNode(functor, trueValues, newArgList);
			arg_ = bestSplit.arg_;
		}
		virtual const T* get(const Key& key)
		{
			if (functor_(key, arg_)) {
				return trueChild_->get(key);
			} else {
				return falseChild_->get(key);
			}
		}
	};

}

template <class Functor, class ValueList, class ArgList>
std::unique_ptr<Node<typename ValueList::value_type::first_type, typename ValueList::value_type::second_type>>
buildNode(
		Functor& functor,
		const ValueList& valueList,
		const ArgList& argList)
{
	typedef typename ValueList::value_type::first_type Key;
	typedef typename ValueList::value_type::second_type T;
	if (valueList.size() == 1) {
		return std::unique_ptr<Node<Key, T>>(new detail::LeafNode<Key, T>(valueList.begin()->first));
	} else
	if (valueList.size() == 0) {
		return std::unique_ptr<Node<Key, T>>(new detail::EmptyLeafNode<Key, T>());
	} else {
		return std::unique_ptr<Node<Key, T>>(
				new detail::DecisionNode<Key, T, typename ArgList::value_type, Functor>(functor, valueList, argList));
	}
}


} // namespace decisionTree



#endif /* DECISIONTREE_H_ */
