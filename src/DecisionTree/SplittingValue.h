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

}



#endif /* SPLITTINGVALUE_H_ */
