#ifndef HEURCALCULATOR_H_
#define HEURCALCULATOR_H_

class Status;

namespace boost {
	template<class T>
	class shared_ptr;
}

class HeurCalculator {
public:
	typedef boost::shared_ptr<HeurCalculator> Ptr;
	virtual ~HeurCalculator() {}
	virtual int calculate(const Status &) const = 0;
	static Ptr create();
};

#endif /* HEURCALCULATOR_H_ */
