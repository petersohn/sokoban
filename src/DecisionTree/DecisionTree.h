#ifndef DECISIONTREE_H_
#define DECISIONTREE_H_

#include <memory>
#include <boost/range/algorithm.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/format.hpp>
#include <boost/optional.hpp>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include "ProgressBar.h"
#include "AnnotatedFunction.h"
#include "DecisionTree/SplittingValue.h"
#include "Dumper/IndentedOutput.h"
#include "Status/Status.h"

namespace decisionTree {


template <class Status, class T>
class Node {
public:
	typedef std::pair<Status, T> Value;
	typedef std::shared_ptr<Value> ValuePtr;
	typedef std::vector<ValuePtr> ValueList;
	virtual const ValueList& get(const Status& key) const = 0;
	virtual ~Node() {}
};

namespace detail {

	const int indentLevel = 2;

	template <class Status, class T>
	class LeafNode: public Node<Status, T> {
		typedef typename Node<Status, T>::Value Value;
		typedef typename Node<Status, T>::ValueList ValueList;
		ValueList value_;
	public:
		LeafNode(const ValueList& value):
			value_(value) {}
		LeafNode(ValueList&& value):
			value_(std::move(value)) {}
		virtual const ValueList& get(const Status&) const
		{
			return value_;
		}
	};

	template <class Status, class T>
	class DecisionNode: public Node<Status, T> {
		typedef typename Node<Status, T>::Value Value;
		typedef typename Node<Status, T>::ValueList ValueList;
		Point point_;

		typedef std::unique_ptr<Node<Status, T>> ChildType;
		ChildType falseChild_;
		ChildType trueChild_;
	public:
		DecisionNode(const Point& point,
				ChildType&& falseChild, ChildType&& trueChild):
			point_(point),
			falseChild_(std::move(falseChild)),
			trueChild_(std::move(trueChild))
		{}

		virtual const ValueList& get(const Status& status) const
		{
			if (isStone(status, point_)) {
				return trueChild_->get(status);
			} else {
				return falseChild_->get(status);
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

		template <class ValueList, class PointList>
		boost::optional<Point>
		filterPointList(
				const ValueList& valueList,
				const PointList& pointList,
				std::vector<Point>& newPointList) const
		{
			typedef typename PointList::const_iterator PointIterator;
			typedef std::vector<SplittingValue<PointIterator>> SplittingValues;
			typedef typename SplittingValues::const_iterator SplittingValueIterator;

			SplittingValues splittingValues;
			splittingValues.reserve(valueList.size());
			for (PointIterator it = std::begin(pointList);
					it != std::end(pointList); ++it) {
				auto splittingValue = calculateSplittingValue(it, valueList);
				splittingValues.push_back(std::move(splittingValue));
			}
			boost::sort(splittingValues, betterSplittingValue<PointIterator>);

			if (splittingValues.empty() || splittingValues.front().trueNum_ == 0) {
				return boost::optional<Point>();
			}
			Point result = *splittingValues.front().arg_;
			SplittingValueIterator begin = ++splittingValues.begin();
			SplittingValueIterator end = boost::find_if(splittingValues,
					[](const SplittingValue<PointIterator>& value)
					{ return value.trueNum_ == 0; });
			if (begin != end) {
				newPointList.reserve(std::distance(begin, end));
				std::transform(begin, end, std::back_inserter(newPointList),
						[](const SplittingValue<PointIterator>& value)
						{ return *(value.arg_); });
			}
			return result;
		} // filterFunctorList

		template <class PointList>
		boost::optional<Point>
		fastFilterPointList(
				const PointList& pointList,
				std::vector<Point>& newPointList) const
		{
			typedef typename PointList::value_type Functor;

			assert(!pointList.empty());
			newPointList.reserve(pointList.size() - 1);
			std::copy(
					++pointList.begin(),
					pointList.end(),
					std::back_inserter(newPointList)
					);
			return boost::optional<Point>(pointList.front());
		} // fastFilterFunctorList

		template <class Status, class T>
		std::unique_ptr<Node<Status, T>>
		createLeaf(
				typename Node<Status, T>::ValueList&& valueList,
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
			return std::unique_ptr<Node<Status, T>>(
						new detail::LeafNode<Status, T>(std::move(valueList)));
		}

		template <class Key, class T, class PointList>
		std::unique_ptr<Node<Key, T>>
		doBuildNode(
			typename Node<Key, T>::ValueList&& valueList,
			const PointList& pointList,
			int depthRemaining,
			bool trueBranch)
		{
			typedef typename Node<Key, T>::ValuePtr ValuePtr;
			typedef typename Node<Key, T>::ValueList ValueList;

			if (valueList.size() == 0 ||
					pointList.size() == 0 ||
					depthRemaining == 0) {
				return createLeaf<Key, T>(std::move(valueList), depthRemaining);
			} else {
				std::vector<Point> newFunctorList;
				boost::optional<Point> point;
				if (trueBranch) {
					point = fastFilterPointList(
							pointList,
							newFunctorList);
				} else {
					point = filterPointList(
							valueList,
							pointList,
							newFunctorList);
				}
				if (!point) {
					return createLeaf<Key, T>(std::move(valueList), depthRemaining);
				}

				ValueList falseValues;
				boost::remove_copy_if(valueList,
						std::back_inserter(falseValues),
						[&point](const ValuePtr& value)
						{ return isStone(value->first, *point); });

				assert(falseValues.size() != valueList.size());
				return std::unique_ptr<Node<Key, T>>(
						new detail::DecisionNode<Key, T>(
								*point,
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

		template <class Key, class T, class PointList>
		std::unique_ptr<Node<Key, T>>
		buildNode(
			typename Node<Key, T>::ValueList&& valueList,
			const PointList& pointList)
		{
			return doBuildNode<Key, T>(
					std::move(valueList),
					pointList,
					maxDepth_,
					false);
		}

	}; // class NodeBuilder

} // namespace detail

template <class Key, class T, class PointList>
std::unique_ptr<Node<Key, T>>
buildNode(
		typename Node<Key, T>::ValueList&& valueList,
		const PointList& pointList,
		int maxDepth
		)
	{
		std::cerr << "Building decision tree\n";
		return detail::NodeBuilder(maxDepth).buildNode<Key, T>(
				std::move(valueList),
				pointList);
	} // buildNode


} // namespace decisionTree



#endif /* DECISIONTREE_H_ */
