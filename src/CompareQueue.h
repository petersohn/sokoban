#ifndef COMPAREQUEUE_H_
#define COMPAREQUEUE_H_

#include "ComplexStrategy.h"
#include <utility>
#include <boost/lambda/lambda.hpp>

template<class T>
class CompareQueue {
public:
private:
	typedef ComplexStrategy<std::function<int(const T&, const T&)> > StrategyType;
	StrategyType strategy_;
public:
	typedef typename StrategyType::FuncType FuncType;

	template<class Iterator>
	CompareQueue(Iterator first, Iterator last):
			strategy_(first, last,
					boost::lambda::_2,
					boost::lambda::_1 != 0,
					0)
	{
	}

	bool operator()(const T& first, const T& second) {
		return strategy_.execute(first, second) > 0;
	}
};

template<class T>
class CompareByMethod {
	typedef std::function<int(const T&)> Fun;

	Fun method_; // ha-ha
	bool backwards_;
public:
	CompareByMethod(Fun method, bool backwards = false):
		method_(method),
		backwards_(backwards)
	{}
	int operator()(const T &a, const T &b) {
		return backwards_ ?
				method_(b) - method_(a) :
				method_(a) - method_(b);
	}
};

template<class Ptr>
class CompareByMethodPtr {
public:
	typedef std::function<int(const typename Ptr::element_type&)> Fun;
private:
	Fun method_; // ha-ha
	bool backwards_;
public:
	CompareByMethodPtr(Fun method, bool backwards = false):
		method_(method),
		backwards_(backwards)
	{}
	int operator()(Ptr a, Ptr b) {
		return backwards_ ?
				method_(*b) - method_(*a) :
				method_(*a) - method_(*b);
	}
};

#endif /* COMPAREQUEUE_H_ */
