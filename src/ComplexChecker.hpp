#ifndef COMPLEXCHECKER_H_
#define COMPLEXCHECKER_H_

#include "Checker.hpp"
#include <vector>

class ComplexChecker {
	typedef std::vector<std::shared_ptr<const Checker>> ContainerType;
	ContainerType funcs_;
#ifndef NO_UNSAFE_DIAGNOSTICS
	mutable const char *lastError_;
#endif
public:
	ComplexChecker() = default;

	template<class Range>
	ComplexChecker(const Range& range):
		funcs_(range.begin(), range.end())
	{}
	ComplexChecker(const std::initializer_list<std::shared_ptr<const Checker>>& range):
		funcs_(range.begin(), range.end())
	{}

	ComplexChecker(const ComplexChecker&) = default;
	ComplexChecker& operator=(const ComplexChecker&) = default;
	ComplexChecker(ComplexChecker&&) = default;
	ComplexChecker& operator=(ComplexChecker&&) = default;

	void append(std::shared_ptr<const Checker> checker)
	{
		funcs_.push_back(std::move(checker));
	}

	bool check(const Status &status, Point p0) const
	{
#ifndef NO_UNSAFE_DIAGNOSTICS
		lastError_ = "";
#endif
		for (const auto& func: funcs_) {
			assert(func.get() != NULL);
			if (!func->check(status, p0)) {
#ifndef NO_UNSAFE_DIAGNOSTICS
				lastError_ = func->errorMessage();
#endif
				return false;
			}
		}
		return true;
	}

	const char* errorMessage() const
	{
#ifndef NO_UNSAFE_DIAGNOSTICS
		return lastError_;
#else
		return "check failed";
#endif
	}
};

#endif /* COMPLEXCHECKER_H_ */
