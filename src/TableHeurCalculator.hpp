#ifndef TABLEHEURCALCULATOR_H_
#define TABLEHEURCALCULATOR_H_

#include "HeurCalculator.hpp"
#include "Array.hpp"
#include "Status/Status.hpp"
#include "Status/Table.hpp"
#include "Mutexes.hpp"
#include <boost/scoped_ptr.hpp>
#include <boost/thread/locks.hpp>

class TableHeurCalculator: public HeurCalculator {
	FixedTable::Ptr table_;
	MutexType heurMutex_;
	virtual void init() = 0;
	virtual int doCalculateStone(const Status &status, Point p) = 0;
	void checkTable(const Status &status) {
		boost::lock_guard<MutexType> lck(heurMutex_);
		if (table_ != status.tablePtr()) {
			std::cerr << table_ << " --> " << status.tablePtr() << std::endl;
			setTable(status.tablePtr());
		}
	}
public:
	TableHeurCalculator():
		MUTEX_DECL(heurMutex_)
	{}
	virtual int calculateStatus(
			const Status &status,
			const MoveDescriptor* moveDescriptor,
			const std::shared_ptr<Node>& ancestor);
	virtual int calculateStone(const Status &status, Point p);

	void setTable(const FixedTable::Ptr& table)
	{
		table_ = table;
		init();
	}
	const Table& table() const { return table_->get(); }
	FixedTable::Ptr tablePtr() const { return table_; }
};

#endif /* TABLEHEURCALCULATOR_H_ */
