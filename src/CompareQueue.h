#ifndef COMPAREQUEUE_H_
#define COMPAREQUEUE_H_

#include "ComplexStrategy.h"
#include <utility>
#include <boost/lambda/lambda.hpp>

template<class T>
class CompareQueue {
public:
	typedef std::pair<const T&, const T&> InputType;
private:
	typedef ComplexStrategy<InputType, int, 0> StrategyType;
	StrategyType strategy_;
public:
	typedef typename StrategyType::FuncType FuncType;

	template<class Iterator>
	CompareQueue(Iterator first, Iterator last):
			strategy_(first, last,
					boost::lambda::_2,
					boost::lambda::_1 != 0)
	{
	}

	bool operator()(const T& first, const T& second) {
		return strategy_(std::make_pair(first, second)) > 0;
	}
};

template<class T>
class CompareByMethod {
	typedef boost::function<int(T)> Fun;

	Fun method_; // ha-ha
	bool backwards_;
public:
	CompareByMethod(Fun method, bool backwards = false):
		method_(method),
		backwards_(backwards)
	{}
	int operator()(typename CompareQueue<T>::InputType input) {
		return backwards_ ?
				method_(input.first) - method_(input.second) :
				method_(input.second) - method_(input.first);
	}
};

#endif /* COMPAREQUEUE_H_ */
