#ifndef TABLEHEURCALCULATOR_H_
#define TABLEHEURCALCULATOR_H_

#include "HeurCalculator.h"
#include "Array.h"
#include "Status.h"
#include "Table.h"
#include <boost/scoped_ptr.hpp>
#include <boost/thread/locks.hpp>

class TableHeurCalculator: public HeurCalculator {
	FixedTable::Ptr table_;
	MutexType mtx_;
	virtual void init() = 0;
	virtual int doCalculateStone(const Status &status, const Point &p) = 0;
	void checkTable(const Status &status) {
		boost::lock_guard<MutexType> lck(mtx_);
		if (table_ != status.tablePtr()) {
			table_ = status.tablePtr();
			init();
		}
	}
public:
	TableHeurCalculator() {}
	virtual int calculateStatus(const Status &status);
	virtual int calculateStone(const Status &status, const Point &p);

	const Table& table() const { return table_->get(); }
	FixedTable::Ptr tablePtr() const { return table_; }
};

#endif /* TABLEHEURCALCULATOR_H_ */
