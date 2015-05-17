#ifndef SPLITTINGVALUE_H_
#define SPLITTINGVALUE_H_

namespace decisionTree {

template <typename Arg>
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

template <typename ValueList, typename PointIterator>
SplittingValue<PointIterator> calculateSplittingValue(
        PointIterator& iterator,
        const ValueList& valueList)
{
    int trueNum = 0;
    int falseNum = 0;
    for (const auto& value: valueList) {
        if (isStone(value->first, *iterator)) {
            ++trueNum;
        } else {
            ++falseNum;
        }
    }
    return SplittingValue<PointIterator>(iterator, trueNum, falseNum);
}

}



#endif /* SPLITTINGVALUE_H_ */
