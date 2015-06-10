#ifndef NODEBUILDER_H_
#define NODEBUILDER_H_

#include "Checker/ComplexCheckerBase.hpp"
#include "Checker/ComplexCheckerFwd.hpp"

#include "DecisionTree/Node.hpp"
#include "DecisionTree/SplittingValue.hpp"

#include "Dumper/IndentedOutput.hpp"

#include "Status/StatusUtils.hpp"

#include "Mutexes.hpp"
#include "ProgressBar.hpp"

#include "util/ThreadPool.hpp"

#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/optional.hpp>
#include <boost/thread.hpp>

#include <cstdlib>
#include <iostream>
#include <iterator>

namespace decisionTree {

namespace detail {

    const int indentLevel = 2;

    class NodeBuilder {
        int maxDepth_;
        ProgressBar progressBar_;
        int progress_;
        boost::optional<ComplexChecker> checker_;
        util::ThreadPool threadPool_;
        int numThreads_;

        size_t maxLength_;
        size_t minLength_;
        size_t sumLength_;
        size_t numNonemptyLeafs_;
        size_t numEmptyLeafs_;
        size_t numFullDepthLeafs_;
        size_t numLeafsSaved_;
        size_t numLeafsSavedExp_;
        MutexType MUTEX_DECL(progressMutex_);

        void advanceProgress(size_t size, int depthRemaining)
        {
            boost::unique_lock<MutexType> lock(progressMutex_);
            maxLength_ = std::max(maxLength_, size);
            sumLength_ += size;
            if (size == 0) {
                ++numEmptyLeafs_;
            } else {
                minLength_ = std::min(minLength_, size);
                ++numNonemptyLeafs_;
            }
            if (depthRemaining == 0) {
                ++numFullDepthLeafs_;
            }
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
        boost::optional<Point> fastFilterPointList(
                const PointList& pointList,
                std::vector<Point>& newPointList) const
        {
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
        Node<Status, T> createLeaf(
                const typename NodeTypes<Status, T>::ValueList& originalValueList,
                int depthRemaining,
                const State& collectedState)
        {
            typedef typename NodeTypes<Status, T>::ValuePtr ValuePtr;
            typedef typename NodeTypes<Status, T>::ValueList ValueList;

            ValueList valueList;
            if (checker_) {
                boost::remove_copy_if(
                        originalValueList,
                        std::back_inserter(valueList),
                        [this, &collectedState](const ValuePtr& value)
                        {
                            State state(collectedState);
                            for (Point  p: value->first.state()) {
                                state.addStone(p);
                            }
                            return !checkState(*checker_, value->first.table(), state);
                        });
            } else {
                valueList = originalValueList;
            }
            advanceProgress(valueList.size(), depthRemaining);
            return LeafNode<Status, T>(std::move(valueList));
        }

        template <class Status, class T, class PointList>
        void buildDecisionChildren(
            const typename NodeTypes<Status, T>::ValueList& falseValues,
            const typename NodeTypes<Status, T>::ValueList& trueValues,
            const PointList& pointList,
            int depthRemaining,
            const State& falseCollectedState,
            const State& trueCollectedState,
            Node<Status, T>& result)
        {
            auto& resultNode = boost::get<DecisionNode<Status, T, Node<Status, T>>&>(result);
            if (numThreads_ > 1) {
                threadPool_.getIoService().post(std::bind(
                        &NodeBuilder::doBuildNode<Status, T, std::vector<Point>>,
                        this, falseValues, pointList, depthRemaining,
                        false, falseCollectedState, std::ref(resultNode.falseChild())));
                threadPool_.getIoService().post(std::bind(
                        &NodeBuilder::doBuildNode<Status, T, std::vector<Point>>,
                        this, trueValues, pointList, depthRemaining,
                        true, trueCollectedState, std::ref(resultNode.trueChild())));
            } else {
                doBuildNode<Status, T>(
                        falseValues, pointList, depthRemaining,
                        false, falseCollectedState, resultNode.falseChild());
                doBuildNode<Status, T>(
                        trueValues, pointList, depthRemaining,
                        true, trueCollectedState, resultNode.trueChild());
            }
        }

        template <class Status, class T, class PointList>
        void doBuildNode(
            const typename NodeTypes<Status, T>::ValueList& valueList,
            const PointList& pointList,
            int depthRemaining,
            bool trueBranch,
            const State& collectedState,
            Node<Status, T>& result)
        {
            typedef typename NodeTypes<Status, T>::ValuePtr ValuePtr;
            typedef typename NodeTypes<Status, T>::ValueList ValueList;

            if (valueList.empty() ||
                    pointList.empty() ||
                    depthRemaining == 0) {
                result = createLeaf<Status, T>(valueList, depthRemaining, collectedState);
                return;
            }
            assert(!valueList.empty());
            if (checker_ && !checkState(
                    *checker_,
                    valueList.front()->first.table(),
                    collectedState)) {
                {
                    boost::unique_lock<MutexType> lock(progressMutex_);
                    ++numLeafsSaved_;
                    numLeafsSavedExp_ += static_cast<int>(exp2(depthRemaining));
                }
                result = createLeaf<Status, T>(ValueList(), depthRemaining, collectedState);
                return;
            }

            std::vector<Point> newFunctorList;
            boost::optional<Point> point;
            State newCollectedState(collectedState);
            if (trueBranch) {
                point = fastFilterPointList(
                        pointList, newFunctorList);
                assert(point);
            } else {
                point = filterPointList(
                        valueList, pointList, newFunctorList);
            }
            if (!point) {
                result = createLeaf<Status, T>(valueList, depthRemaining, collectedState);
                return;
            }
            newCollectedState.addStone(*point);

            ValueList falseValues;
            boost::remove_copy_if(valueList,
                    std::back_inserter(falseValues),
                    [&point](const ValuePtr& value)
                    { return isStone(value->first, *point); });

            assert(falseValues.size() != valueList.size());
            result = DecisionNode<Status, T, Node<Status, T>>(*point);
            buildDecisionChildren<Status, T, PointList>(
                    falseValues, valueList,
                    newFunctorList, depthRemaining - 1,
                    collectedState, newCollectedState,
                    result);

        } // doBuildNode
    public:
        NodeBuilder(int maxDepth, boost::optional<ComplexChecker> checker, int numThreads):
            maxDepth_(maxDepth),
            progressBar_(static_cast<int>(exp2(maxDepth))),
            progress_(0),
            checker_(std::move(checker)),
            numThreads_(numThreads),
            maxLength_(0),
            minLength_(0),
            sumLength_(0),
            numNonemptyLeafs_(0),
            numEmptyLeafs_(0),
            numFullDepthLeafs_(0),
            numLeafsSaved_(0),
            numLeafsSavedExp_(0)
        {
            threadPool_.setNumThreads(numThreads);
        }

        ~NodeBuilder()
        {
            std::cerr << "\nMaximum leaf length: " << maxLength_ << "\n"
                    "Minimum non-empty leaf length: " << minLength_ << "\n"
                    "Average leaf length: " <<
                    static_cast<double>(sumLength_) / numNonemptyLeafs_ << "\n"
                    "Number of leaves saved by checker: " << numLeafsSaved_ <<
                    " (" << numLeafsSavedExp_ << ")\n"
                    "Number of empty leaves: " << numEmptyLeafs_ << "\n"
                    "Number of non-empty leaves: " << numNonemptyLeafs_ << "\n"
                    "Number of full depth leaves: " << numFullDepthLeafs_;
        }

        template <class Key, class T, class PointList>
        Node<Key, T> buildNode(
            const typename NodeTypes<Key, T>::ValueList& valueList,
            const PointList& pointList)
        {
            minLength_ = pointList.size();
            Node<Key, T> result;
            {
                util::ThreadPoolRunner runner(threadPool_);
                doBuildNode<Key, T>(
                        valueList,
                        pointList,
                        maxDepth_,
                        false,
                        State(),
                        result);
            }
            return result;
        }

    }; // class NodeBuilder

} // namespace detail

} // namespace decisionTree



#endif /* NODEBUILDER_H_ */
