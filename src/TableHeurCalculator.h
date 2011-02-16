#ifndef TABLEHEURCALCULATOR_H_
#define TABLEHEURCALCULATOR_H_

#include "HeurCalculator.h"
#include "Array.h"
#include "Table.h"
#include <boost/scoped_ptr.hpp>

class TableHeurCalculator: public HeurCalculator {
	FixedTable::Ptr table_;
	virtual void init() = 0;
	virtual int doCalculateStone(const Status &status, const Point &p) = 0;
public:
	TableHeurCalculator() {}
	virtual int calculateStatus(const Status &status);
	virtual int calculateStone(const Status &status, const Point &p);

	const Table& table() const { return table_->get(); }
	FixedTable::Ptr tablePtr() const { return table_; }
};

#endif /* TABLEHEURCALCULATOR_H_ */
