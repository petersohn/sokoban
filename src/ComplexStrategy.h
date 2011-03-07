#ifndef COMPLEXSTRATEGY_H_
#define COMPLEXSTRATEGY_H_

#include <vector>
#include <boost/function.hpp>

template<class Fun>
class ComplexStrategy {
public:
	typedef Fun FuncType;
	typedef typename Fun::result_type result_type;
	typedef boost::function<bool(const result_type&)> StopConditionType;
	typedef boost::function<result_type(const result_type&, const result_type&)> AggregateType;
private:
	typedef std::vector<FuncType> FuncListType;
	FuncListType funcs_;
	AggregateType aggregate_;
	StopConditionType stopCondition_;
	result_type init_;
public:
	template<class Iterator>
	ComplexStrategy(Iterator first, Iterator last,
			AggregateType aggregate, StopConditionType stopCondition, const result_type &init):
			funcs_(first, last),
			aggregate_(aggregate),
			stopCondition_(stopCondition),
			init_(init)
	{}

	result_type execute() {
		result_type result = init_;
		for (typename std::vector<FuncType >::iterator it = funcs_.begin();
				it != funcs_.end(); ++it) {
			if (stopCondition_(result))
				return result;
			result = aggregate_(result, (*it)());
		}
		return result;
	}

	template<class T1>
	result_type execute(T1 t1) {
		result_type result = init_;
		for (typename std::vector<FuncType >::iterator it = funcs_.begin();
				it != funcs_.end(); ++it) {
			if (stopCondition_(result))
				return result;
			result = aggregate_(result, (*it)(t1));
		}
		return result;
	}

	template<class T1, class T2>
	result_type execute(T1 t1, T2 t2) {
		result_type result = init_;
		for (typename std::vector<FuncType >::iterator it = funcs_.begin();
				it != funcs_.end(); ++it) {
			if (stopCondition_(result))
				return result;
			result = aggregate_(result, (*it)(t1, t2));
		}
		return result;
	}

	template<class T1, class T2, class T3>
	result_type execute(T1 t1, T2 t2, T3 t3) {
		result_type result = init_;
		for (typename std::vector<FuncType >::iterator it = funcs_.begin();
				it != funcs_.end(); ++it) {
			if (stopCondition_(result))
				return result;
			result = aggregate_(result, (*it)(t1, t2, t3));
		}
		return result;
	}

};

#endif /* COMPLEXSTRATEGY_H_ */
