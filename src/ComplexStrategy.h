#ifndef COMPLEXSTRATEGY_H_
#define COMPLEXSTRATEGY_H_

#include <vector>
#include <boost/function.hpp>

template<class Param, class Return, Return _init>
class ComplexStrategy {
public:
	typedef boost::function<Return(Param)> FuncType;
	typedef boost::function<bool(const Return&)> StopConditionType;
	typedef boost::function<Return(const Return&, const Return&)> AggregateType;
private:
	typedef std::vector<FuncType> FuncListType;
	FuncListType funcs_;
	AggregateType aggregate_;
	StopConditionType stopCondition_;
public:
	template<class Iterator>
	ComplexStrategy(Iterator first, Iterator last,
			AggregateType aggregate, StopConditionType stopCondition):
			funcs_(first, last),
			aggregate_(aggregate),
			stopCondition_(stopCondition) {}
	Return operator()(Param param);
};

template<class Param, class Return, Return _init>
Return ComplexStrategy<Param, Return, _init>::operator()(Param param) {
	Return result = _init;
	FuncType *p;
	for (typename std::vector<FuncType >::iterator it = funcs_.begin();
			it != funcs_.end(); ++it) {
		if (stopCondition_(result))
			return result;
		result = aggregate_(result, (*it)(param));
	}
	return result;
}

#endif /* COMPLEXSTRATEGY_H_ */
