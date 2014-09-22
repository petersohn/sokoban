#ifndef ANNOTATEDFUNCTION_H_
#define ANNOTATEDFUNCTION_H_

#include <functional>
#include <algorithm>
#include <string>

template <class Signature>
class AnnotatedFunction;

template <class Return, class... Args>
class AnnotatedFunction<Return(Args...)> {
	std::function<Return(Args...)> function_;
	std::string name_;
public:
	AnnotatedFunction() {}

	template <class OtherSignature>
	AnnotatedFunction(const AnnotatedFunction<OtherSignature>& other):
		function_(other.function_),
		name_(other.name)
	{}
	template <class OtherSignature>
	AnnotatedFunction(AnnotatedFunction<OtherSignature>&& other):
		function_(std::move(other.function_)),
		name_(std::move(other.name))
	{}
	template <class T>
	AnnotatedFunction(T&& fun):
		function_(std::forward<T>(fun))
		{}

	const std::string& name() const { return name_; }
	void name(const std::string& value) { name_ = value; }

	Return operator()(Args&&... args) const
	{
		return function_(std::forward<Args>(args)...);
	}
};

template <class Signature>
const std::string& functionName(const AnnotatedFunction<Signature>& fun)
{
	return fun.name();
}

template <class T>
const std::string& functionName(const T&)
{
	static std::string result("Unknown function");
	return result;
}



#endif /* ANNOTATEDFUNCTION_H_ */
