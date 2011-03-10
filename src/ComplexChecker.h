#ifndef COMPLEXCHECKER_H_
#define COMPLEXCHECKER_H_

#include "Checker.h"
#include <vector>

class ComplexChecker: public Checker {
	typedef std::vector<Checker::Ptr> ContainerType;
	ContainerType funcs_;
	const char *lastError_;
public:
	template<class Iterator>
	ComplexChecker(Iterator first, Iterator last):
		funcs_(first, last)
	{}
	virtual bool check(const Status &status, const Point &p0);
	virtual const char* errorMessage();
};

#endif /* COMPLEXCHECKER_H_ */
