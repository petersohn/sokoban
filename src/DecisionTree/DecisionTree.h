#ifndef DECISIONTREE_H_
#define DECISIONTREE_H_

#include <memory>
#include <boost/range/algorithm.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/format.hpp>
#include <vector>
#include <cstdlib>
#include <iostream>
#include "ProgressBar.h"
#include "AnnotatedFunction.h"
#include "DecisionTree/SplittingValue.h"
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
		std::shared_ptr<Functor> functor_;

		typedef std::unique_ptr<Node<Key, T>> ChildType;
		ChildType falseChild_;
		ChildType trueChild_;
	public:
		DecisionNode(const std::shared_ptr<Functor>& functor,
				ChildType&& falseChild, ChildType&& trueChild):
			functor_(functor),
			falseChild_(std::move(falseChild)),
			trueChild_(std::move(trueChild))
		{}

		virtual const ValueList& get(const Key& key)
		{
			if ((*functor_)(key)) {
				return trueChild_->get(key);
			} else {
				return falseChild_->get(key);
			}
		}
	}; // class DecisionNode

	class NodeBuilder {
		int maxDepth_;
		ProgressBar progressBar_;
		int progress_;

		void advanceProgress(int depthRemaining)
		{
			progress_ += static_cast<int>(exp2(depthRemaining));
			progressBar_.draw(progress_);
		}

		template <class ValueList, class FunctorList>
		std::shared_ptr<typename FunctorList::value_type>
		filterFunctorList(
				const ValueList& valueList,
				const FunctorList& functorList,
				std::vector<typename FunctorList::value_type>& newFunctorList) const
		{
			typedef typename FunctorList::value_type Functor;
			typedef typename FunctorList::const_iterator FunctorIterator;
			typedef std::vector<SplittingValue<FunctorIterator>> SplittingValues;
			typedef typename SplittingValues::const_iterator SplittingValueIterator;

			SplittingValues splittingValues;
			splittingValues.reserve(valueList.size());
			for (FunctorIterator it = std::begin(functorList);
					it != std::end(functorList); ++it) {
				auto splittingValue = calculateSplittingValue(it, valueList);
				splittingValues.push_back(std::move(splittingValue));
			}
			boost::sort(splittingValues, betterSplittingValue<FunctorIterator>);

			std::shared_ptr<Functor> result;
			if (splittingValues.empty()) {
				return result;
			}
			result = std::make_shared<Functor>(*splittingValues.front().arg_);
			SplittingValueIterator begin = ++splittingValues.begin();
			SplittingValueIterator end = boost::find_if(splittingValues,
					[](const SplittingValue<FunctorIterator>& value)
					{ return value.trueNum_ == 0; });
			newFunctorList.reserve(end - begin);
			std::transform(begin, end, std::back_inserter(newFunctorList),
					[](const SplittingValue<FunctorIterator>& value)
					{ return *(value.arg_); });

			return result;
		}

		template <class Key, class T, class FunctorList>
		std::unique_ptr<Node<Key, T>>
		doBuildNode(
			typename Node<Key, T>::ValueList&& valueList,
			const FunctorList& functorList,
			int depthRemaining)
		{
			typedef typename Node<Key, T>::ValuePtr ValuePtr;
			typedef typename Node<Key, T>::ValueList ValueList;
			typedef typename FunctorList::value_type Functor;

			if (valueList.size() == 0 ||
					functorList.size() == 0 ||
					depthRemaining == 0) {
				advanceProgress(depthRemaining);
				return std::unique_ptr<Node<Key, T>>(
						new detail::LeafNode<Key, T>(std::move(valueList)));
			} else {
				std::vector<Functor> newFunctorList;
				std::shared_ptr<Functor> functor = filterFunctorList(
						valueList,
						functorList,
						newFunctorList);
				if (!functor) {
					advanceProgress(depthRemaining);
					return std::unique_ptr<Node<Key, T>>(
							new detail::LeafNode<Key, T>(std::move(valueList)));
				}

				ValueList falseValues;
				boost::remove_copy_if(valueList,
						std::back_inserter(falseValues),
						[&functor](const ValuePtr& value)
						{ return (*functor)(value->first); });

				assert(falseValues.size() != valueList.size());
				return std::unique_ptr<Node<Key, T>>(
						new detail::DecisionNode<Key, T, Functor>(
								functor,
								doBuildNode<Key, T>(std::move(falseValues),
										newFunctorList,
										depthRemaining - 1),
								doBuildNode<Key, T>(std::move(valueList),
										newFunctorList,
										depthRemaining - 1)
						));
			}
		} // doBuildNode
	public:
		NodeBuilder(int maxDepth):
			maxDepth_(maxDepth),
			progressBar_(static_cast<int>(exp2(maxDepth))),
			progress_(0)
		{}

		template <class Key, class T, class FunctorList>
		std::unique_ptr<Node<Key, T>>
		buildNode(
			typename Node<Key, T>::ValueList&& valueList,
			const FunctorList& functorList)
		{
			return doBuildNode<Key, T>(
					std::move(valueList),
					functorList,
					maxDepth_);
		}

	}; // class NodeBuilder

} // namespace detail

template <class Key, class T, class FunctorList>
std::unique_ptr<Node<Key, T>>
buildNode(
		typename Node<Key, T>::ValueList&& valueList,
		const FunctorList& functorList,
		int maxDepth
		)
	{
		std::cerr << "Building decision tree\n";
		return detail::NodeBuilder(maxDepth).buildNode<Key, T>(
				std::move(valueList),
				functorList);
	} // buildNode


} // namespace decisionTree



#endif /* DECISIONTREE_H_ */
