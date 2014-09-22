#ifndef TEMPVALUE_H_
#define TEMPVALUE_H_

#include <boost/noncopyable.hpp>

template <class T>
class TempValue: public boost::noncopyable {
	T& variable_;
	T oldValue_;
public:
	TempValue(T& variable, const T& value):
		variable_(variable),
		oldValue_(variable)
	{
		variable = value;
	}
	~TempValue()
	{
		variable_ = oldValue_;
	}
};


#endif /* TEMPVALUE_HPP_ */
