#ifndef COMPLEXCHECKER_H_
#define COMPLEXCHECKER_H_

#include "Checker.hpp"
#include <vector>

class ComplexChecker: public Checker {
	typedef std::vector<Checker::Ptr> ContainerType;
	ContainerType funcs_;
#ifndef NO_UNSAFE_DIAGNOSTICS
	mutable const char *lastError_;
#endif
public:
	template<class Range>
	ComplexChecker(const Range& range):
		funcs_(range.begin(), range.end())
	{}
	bool check(const Status &status, Point p0) const override;
	const char* errorMessage() const override;
};

#endif /* COMPLEXCHECKER_H_ */
