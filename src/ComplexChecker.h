#ifndef COMPLEXCHECKER_H_
#define COMPLEXCHECKER_H_

#include "Checker.h"
#include "ComplexStrategy.h"
#include "boost/lambda/lambda.hpp"

class ComplexChecker {
	struct CheckParams {
		const Status &status;
		const Point &p;
		CheckParams(const Status &status, const Point &p):
			status(status),
			p(p) {}
	};

	ComplexStrategy<CheckParams&, bool, true> funcs_;
public:
	template<class Iterator>
	ComplexChecker(Iterator first, Iterator last):
		funcs_(first, last,
				boost::lambda::_1 && boost::lambda::_2,
				! boost::lambda::_1)
	{}
	virtual bool check(const Status &status, const Point &p0);
};

#endif /* COMPLEXCHECKER_H_ */
