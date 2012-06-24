#ifndef OPTIMALSPLITTING_H_
#define OPTIMALSPLITTING_H_

#include "DecisionTree/DecisionTree.h"

namespace decisionTree {

namespace detail {

	template <class Arg>
	bool betterSplittingValue(const SplittingValue<Arg>& left, const SplittingValue<Arg>& right)
	{
		return std::abs(left.trueNum_ - left.falseNum_) <
				std::abs(right.trueNum_ - right.falseNum_);
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

}

class OptimalSplitting {
public:

	template <class Functor, class ValueList, class ArgList>
	SplittingValue<typename ArgList::value_type> operator()(
			Functor& functor,
			const ValueList& valueList,
			const ArgList& argList) const
	{
		typedef typename ArgList::value_type Arg;
		std::vector<SplittingValue<Arg>> splittingValues;
		splittingValues.reserve(valueList.size());
		boost::transform(
				argList, std::back_inserter(splittingValues),
				[&functor, &valueList](const Arg& arg)
				{
					return detail::calculateSplittingValue(functor, valueList, arg);
				});
		return *boost::min_element(splittingValues, detail::betterSplittingValue<Arg>);
	}
};

}



#endif /* OPTIMALSPLITTING_H_ */
