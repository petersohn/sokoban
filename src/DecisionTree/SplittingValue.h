#ifndef SPLITTINGVALUE_H_
#define SPLITTINGVALUE_H_

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


template <class Arg>
bool betterSplittingValue(const SplittingValue<Arg>& left, const SplittingValue<Arg>& right)
{
	return left.trueNum_ > right.trueNum_;
}

template <class ValueList, class FunctorIterator>
SplittingValue<FunctorIterator> calculateSplittingValue(
		FunctorIterator& functor,
		const ValueList& valueList)
{
	int trueNum = 0;
	int falseNum = 0;
	for (const auto& value: valueList) {
		if ((*functor)(value->first)) {
			++trueNum;
		} else {
			++falseNum;
		}
	}
	return SplittingValue<FunctorIterator>(functor, trueNum, falseNum);
}

}



#endif /* SPLITTINGVALUE_H_ */
