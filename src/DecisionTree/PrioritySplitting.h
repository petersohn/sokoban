#ifndef PRIORITYSPLITTING_H_
#define PRIORITYSPLITTING_H_

#include "DecisionTree/DecisionTree.h"


namespace decisionTree {

//template <class ProxyAlgorithm>
//class PrioritySplitting {
//	ProxyAlgorithm proxyAlgorithm_;
//public:
//	PrioritySplitting(const ProxyAlgorithm& proxyAlgorithm = ProxyAlgorithm()):
//		proxyAlgorithm_(proxyAlgorithm)
//	{}
//
//	template <class Functor, class ValueList, class ArgList>
//	SplittingValue<typename ArgList::value_type> operator()(
//			Functor& functor,
//			const ValueList& valueList,
//			const ArgList& argList) const
//	{
////		typedef typename ArgList::value_type Arg;
//		typedef typename ValueList::value_type::first_type Key;
////		typedef typename ValueList::value_type::second_type Value;
//
//		const Key& firstKey = valueList.begin()->first;
//		return proxyAlgorithm_(functor, valueList,
//				argList | boost::adaptors::filtered(std::bind(
//						functor,
//						std::cref(firstKey),
//						std::placeholders::_1)));
//	}
//};


}


#endif /* PRIORITYSPLITTING_H_ */
