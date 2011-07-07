#ifndef BLOCKLISTCHECKER_H
#define BLOCKLISTCHECKER_H

#include "Checker.h"
#include "Solver.h"
#include "HeurCalculator.h"
#include "IndexedStatusList.h"
#include "DumperFunctions.h"
#include <fstream>

class BlockListChecker: public Checker {
	Solver::Ptr solver_;
	HeurCalculator::Ptr calculator_;
	Checker::Ptr checker_;
	IndexedStatusList blockList_;
	FixedTable::Ptr table_;
	int numStones_;
	int maxDistance_;
	int solved_;
	int iters_;
	std::ofstream dump_;

	void init();
	void initIter(Point p, int stones, const State &state);
	bool advancePoint(Point &p) {
		if (p.x == table_->get().width() - 1) {
			if (p.y < table_->get().height() - 1) {
				++p.y;
				p.x = 0;
			} else {
				return false;
			}
		} else {
			++p.x;
		}
		return true;
	}
	void dumpStatus(const Status &status, const Point &p, const std::string &title) {
		if (!dump_.good())
			return;
		Array<bool> hl = status.reachableArray();
		hl[p] = true;
		::dumpStatus(dump_, status, title, &hl);
	}
public:
	BlockListChecker(Solver::Ptr solver,
			HeurCalculator::Ptr calculator, Checker::Ptr checker, int numStones, int maxDistance);
	virtual bool check(const Status& status, const Point& p);
	virtual const char* errorMessage();
};

#endif /* BLOCKLISTCHECKER_H */
