#ifndef DECISIONTREE_H_
#define DECISIONTREE_H_

#include <memory>
#include <boost/range/algorithm.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/format.hpp>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <iterator>
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
	virtual const ValueList& get(const Key& key) const = 0;
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
		virtual const ValueList& get(const Key&) const
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

		virtual const ValueList& get(const Key& key) const
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
		size_t maxLength_;
		size_t sumLength_;
		size_t numNonemptyLeafs_;
		size_t numEmptyLeafs_;
		size_t numFullDepthLeafs_;

		void advanceProgress(int depthRemaining)
		{
			progress_ += static_cast<int>(exp2(depthRemaining));
			progressBar_.draw(progress_);
		}

		template <class ValueList, class FunctorPtrList>
		typename FunctorPtrList::value_type
		filterFunctorList(
				const ValueList& valueList,
				const FunctorPtrList& functorList,
				std::vector<typename FunctorPtrList::value_type>& newFunctorList) const
		{
			typedef typename FunctorPtrList::value_type FunctorPtr;
			typedef typename FunctorPtr::element_type Functor;
			typedef typename FunctorPtrList::const_iterator FunctorIterator;
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

			FunctorPtr result;
			if (splittingValues.empty() || splittingValues.front().trueNum_ == 0) {
				return result;
			}
			result = *splittingValues.front().arg_;
			SplittingValueIterator begin = ++splittingValues.begin();
			SplittingValueIterator end = boost::find_if(splittingValues,
					[](const SplittingValue<FunctorIterator>& value)
					{ return value.trueNum_ == 0; });
			if (begin != end) {
				newFunctorList.reserve(std::distance(begin, end));
				std::transform(begin, end, std::back_inserter(newFunctorList),
						[](const SplittingValue<FunctorIterator>& value)
						{ return *(value.arg_); });
			}
			return result;
		} // filterFunctorList

		template <class FunctorPtrList>
		typename FunctorPtrList::value_type
		fastFilterFunctorList(
				const FunctorPtrList& functorList,
				std::vector<typename FunctorPtrList::value_type>& newFunctorList) const
		{
			typedef typename FunctorPtrList::value_type Functor;

			assert(!functorList.empty());
			newFunctorList.reserve(functorList.size() - 1);
			std::copy(
					++functorList.begin(),
					functorList.end(),
					std::back_inserter(newFunctorList)
					);
			return functorList.front();
		} // fastFilterFunctorList

		template <class Key, class T>
		std::unique_ptr<Node<Key, T>>
		createLeaf(
				typename Node<Key, T>::ValueList&& valueList,
				int depthRemaining)
		{
			size_t size = valueList.size();
			maxLength_ = std::max(maxLength_, size);
			sumLength_ += size;
			if (size == 0) {
				++numEmptyLeafs_;
			} else {
				++numNonemptyLeafs_;
			}
			if (depthRemaining == 0) {
				++numFullDepthLeafs_;
			}
			advanceProgress(depthRemaining);
			return std::unique_ptr<Node<Key, T>>(
						new detail::LeafNode<Key, T>(std::move(valueList)));
		}

		template <class Key, class T, class FunctorPtrList>
		std::unique_ptr<Node<Key, T>>
		doBuildNode(
			typename Node<Key, T>::ValueList&& valueList,
			const FunctorPtrList& functorList,
			int depthRemaining,
			bool trueBranch)
		{
			typedef typename Node<Key, T>::ValuePtr ValuePtr;
			typedef typename Node<Key, T>::ValueList ValueList;
			typedef typename FunctorPtrList::value_type FunctorPtr;
			typedef typename FunctorPtr::element_type Functor;

			if (valueList.size() == 0 ||
					functorList.size() == 0 ||
					depthRemaining == 0) {
				return createLeaf<Key, T>(std::move(valueList), depthRemaining);
			} else {
				std::vector<FunctorPtr> newFunctorList;
				FunctorPtr functor;
				if (trueBranch) {
					functor = fastFilterFunctorList(
							functorList,
							newFunctorList);
				} else {
					functor = filterFunctorList(
							valueList,
							functorList,
							newFunctorList);
				}
				if (!functor) {
					return createLeaf<Key, T>(std::move(valueList), depthRemaining);
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
										depthRemaining - 1,
										false),
								doBuildNode<Key, T>(std::move(valueList),
										newFunctorList,
										depthRemaining - 1,
										true)
						));
			}
		} // doBuildNode
	public:
		NodeBuilder(int maxDepth):
			maxDepth_(maxDepth),
			progressBar_(static_cast<int>(exp2(maxDepth))),
			progress_(0),
			maxLength_(0),
			sumLength_(0),
			numNonemptyLeafs_(0),
			numEmptyLeafs_(0),
			numFullDepthLeafs_(0)
		{}

		~NodeBuilder()
		{
			std::cerr << "\nMaximum leaf length: " << maxLength_ << "\n"
					"Average leaf length: " <<
					static_cast<double>(sumLength_) / numNonemptyLeafs_ << "\n"
					"Number of empty leaves: " << numEmptyLeafs_ << "\n"
					"Number of non-empty leaves: " << numNonemptyLeafs_ << "\n"
					"Number of full depth leaves: " << numFullDepthLeafs_ << std::endl;
		}

		template <class Key, class T, class FunctorPtrList>
		std::unique_ptr<Node<Key, T>>
		buildNode(
			typename Node<Key, T>::ValueList&& valueList,
			const FunctorPtrList& functorList)
		{
			return doBuildNode<Key, T>(
					std::move(valueList),
					functorList,
					maxDepth_,
					false);
		}

	}; // class NodeBuilder

} // namespace detail

template <class Key, class T, class FunctorPtrList>
std::unique_ptr<Node<Key, T>>
buildNode(
		typename Node<Key, T>::ValueList&& valueList,
		const FunctorPtrList& functorList,
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
