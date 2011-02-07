#ifndef TABLEHEURCALCULATOR_H_
#define TABLEHEURCALCULATOR_H_

#include "HeurCalculator.h"
#include "Array.h"
#include "Table.h"
#include <boost/scoped_ptr.hpp>

class ProblemHeurCalculator: public HeurCalculator {
	FixedTable::Ptr table_;
	virtual int doCalculate(const Status &status) const = 0;
	virtual void init() = 0;
public:
	ProblemHeurCalculator() {}
	virtual int calculate(const Status &status) const;

	const Table& table() const { return table_->get(); }
	FixedTable::Ptr problemPtr() const { return table_; }
};

#endif /* TABLEHEURCALCULATOR_H_ */
