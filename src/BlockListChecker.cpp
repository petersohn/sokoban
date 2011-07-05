#include "BlockListChecker.h"
#include <algorithm>
#include <boost/foreach.hpp>
#include <iostream>

BlockListChecker::BlockListChecker(Solver::Ptr solver,
		HeurCalculator::Ptr calculator, Checker::Ptr checker, int numStones):
		solver_(solver),
		calculator_(calculator),
		checker_(checker),
		mc_(calculator),
		cc_(calculator),
		numStones_(numStones),
		dump_("blocklist.dump")
{
}

void BlockListChecker::initIter(Point p, int stones, const State &state)
{
	if (++iters_ % 500 == 0) {
		std::cerr << iters_ << std::endl;
	}
	if (state.size() > 0) {
		std::vector<Status::Ptr> parts = getPartitions(table_, state);
		bool ok = false;
		BOOST_FOREACH(Status::Ptr status, parts) {
			int heur = calculator_->calculateStatus(*status);
			if (heur < 0) {
//				dumpStatus(*status, p, "Dropped: bad stone");
				continue;
			}
			bool ok2 = true;
			BOOST_FOREACH(const Point &pp, state) {
//				if (!checker_->check(*status, pp)) {
//					dumpStatus(*status, pp, "Dropped: check failed");
//					ok2 = false;
//					break;
//				}
				if (!mc_.check(*status, pp)) {
					dumpStatus(*status, pp, "Dropped: movable check failed");
					ok2 = false;
					break;
				}
				if (!cc_.check(*status, pp)) {
					dumpStatus(*status, pp, "Dropped: corridor check failed");
					ok2 = false;
					break;
				}
				if (blockList_.hasSubStatus(*status, pp)) {
					dumpStatus(*status, p, "Dropped: already on blocklist");
					ok2 = false;
					break;
				}
			}
			if (!ok2) {
				continue;
			}
			if (blockList_.hasSubStatus(*status, p)) {
//				dumpStatus(*status, p, "Dropped: already on blocklist");
				continue;
			}
			if (stones == 0) {
				++solved_;
				std::deque<Node::Ptr> res = solver_->solve(*status);
				if (res.size() == 0) {
					blockList_.add(*status);
					dumpStatus(*status, p, "Added");
				} else {
//					dumpStatus(*status, "Not added: could solve.");
				}
			} else {
				ok = true;
			}
		}
		if (!ok) {
			return;
		}
		assert(stones > 0);
	}
	do {
		if (!table_->get().wall(p) && !state[p] && p != table_->get().destination()) {
			State state2(state);
			state2.addStone(p);
			initIter(p, stones - 1, state2);
		}
	} while (advancePoint(p));
}

void BlockListChecker::init()
{
	std::cerr << "Calculating block list..." << std::endl;
	blockList_.clear();
	solved_ = 0;
	iters_ = 0;
	State tmp;
	Point p(0, 0);
	for (int n = 2; n <= numStones_; ++n) {
		std::cerr << "Stones = " << n << std::endl;
		initIter(p, n, tmp);
	}
	std::cerr << "Block list calculated. Size = " << blockList_.size() << std::endl;
	dump_ << "Solved: " << solved_ << "\n---------------------" << std::endl;
	dump_.flush();
}

bool BlockListChecker::check(const Status& status, const Point& p)
{
	if (table_ != status.tablePtr()) {
		table_ = status.tablePtr();
		init();
	}
	return !blockList_.hasSubStatus(status, p);
}

const char* BlockListChecker::errorMessage()
{
	return "on blocklist";
}
