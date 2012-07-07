#ifndef OPTIMALSPLITTING_H_
#define OPTIMALSPLITTING_H_

#include "DecisionTree/SplittingValue.h"
#include <boost/range/algorithm.hpp>

namespace decisionTree {

namespace detail {

	template <class Arg>
	bool betterSplittingValue(const SplittingValue<Arg>& left, const SplittingValue<Arg>& right)
	{
		return std::abs(left.trueNum_ - left.falseNum_) <
				std::abs(right.trueNum_ - right.falseNum_);
	}

	template <class ValueList, class FunctorIterator>
	SplittingValue<FunctorIterator> calculateSplittingValue(
			FunctorIterator& functor,
			const ValueList& valueList)
	{
		int trueNum = 0;
		int falseNum = 0;
		for (const auto& value: valueList) {
			if ((*functor)(value.first)) {
				++trueNum;
			} else {
				--falseNum;
			}
			return SplittingValue<FunctorIterator>(functor, trueNum, falseNum);
		}
	}

}

class OptimalSplitting {
public:

	template <class ValueList, class FunctorList>
	typename FunctorList::const_iterator operator()(
			const ValueList& valueList,
			const FunctorList& functorList) const
	{
		typedef typename FunctorList::const_iterator FunctorIterator;
		std::vector<SplittingValue<FunctorIterator>> splittingValues;
		splittingValues.reserve(valueList.size());
		for (FunctorIterator it = functorList.begin();
				it != functorList.end(); ++it) {
			splittingValues.push_back(detail::calculateSplittingValue(it, valueList));
		}
		return boost::min_element(splittingValues,
				detail::betterSplittingValue<FunctorIterator>)->arg_;
	}
};

}



#endif /* OPTIMALSPLITTING_H_ */
