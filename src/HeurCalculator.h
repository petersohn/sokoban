#ifndef HEURCALCULATOR_H_
#define HEURCALCULATOR_H_

class Status;

class HeurCalculator {
public:
	virtual ~HeurCalculator() = 0;
	virtual int calculate(Status &) = 0;
};

#endif /* HEURCALCULATOR_H_ */
