#ifndef COMPLEXCHECKER_H_
#define COMPLEXCHECKER_H_

#include "Checker.hpp"
#include <vector>

class ComplexChecker: public Checker {
	typedef std::vector<Checker::Ptr> ContainerType;
	ContainerType funcs_;
	const char *lastError_;
public:
	template<class Range>
	ComplexChecker(const Range& range):
		funcs_(range.begin(), range.end())
	{}
	virtual bool check(const Status &status, Point p0);
	virtual const char* errorMessage();
};

#endif /* COMPLEXCHECKER_H_ */
