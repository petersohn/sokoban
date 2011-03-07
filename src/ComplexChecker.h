#ifndef COMPLEXCHECKER_H_
#define COMPLEXCHECKER_H_

#include "Checker.h"
#include <vector>

class ComplexChecker: public Checker {
	typedef std::vector<Checker::Ptr> ContainerType;
	ContainerType funcs_;
public:
	template<class Iterator>
	ComplexChecker(Iterator first, Iterator last):
		funcs_(first, last)
	{}
	virtual bool check(const Status &status, const Point &p0);
};

#endif /* COMPLEXCHECKER_H_ */
