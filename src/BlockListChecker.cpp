#include "BlockListChecker.h"
#include <algorithm>
#include <functional>
#include <iostream>
#include <cstdlib>

BlockListChecker::BlockListChecker(Solver::Ptr solver, HeurCalculator::Ptr calculator,
		Checker::Ptr checker, int numStones, int maxDistance, int progressInterval):
		solver_(solver),
		calculator_(calculator),
		checker_(checker),
		numStones_(numStones),
		maxDistance_(maxDistance),
		dump_("blocklist.dump"),
		ioService_(ThreadPool::instance()->ioService()),
		progressInterval_(boost::posix_time::seconds(progressInterval))
{
}

void BlockListChecker::initIter(Point p, int stones, const State &state)
{
	if (state.size() > 0) {
		if (maxDistance_ > 0) {
			for (const Point &pp: state) {
				if (std::abs(p.x - pp.x) > maxDistance_ ||
						std::abs(p.y - pp.y) > maxDistance_) {
					return;
				}
			}
		}
		std::vector<Status::Ptr> parts = getPartitions(table_, state);
		bool ok = false;
		for (Status::Ptr status: parts) {
			int heur = calculator_->calculateStatus(*status);
			if (heur < 0) {
//				dumpStatus(*status, p, "Dropped: bad stone");
				continue;
			}
			bool ok2 = true;
			for (const Point &pp: state) {
				if (!checker_->check(*status, pp)) {
//					dumpStatus(*status, pp, "Dropped: check failed");
					ok2 = false;
					break;
				}
				if (blockList_.hasSubStatus(*status, pp)) {
//					dumpStatus(*status, p, "Dropped: already on blocklist");
					ok2 = false;
					break;
				}
			}
			if (!ok2) {
				continue;
			}
			if (stones == 0) {
				if (++iters_ % 100 == 0) {
					std::cerr << ".";
				}
				ioService_.post(std::bind(&BlockListChecker::doWork, this, status));
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

void BlockListChecker::doWork(Status::Ptr status)
{
	std::deque<Node::Ptr> res = solver_->solve(*status);
	if (res.size() == 0) {
		{
			boost::lock_guard<MutexType> lck(listMutex_);
			blockList_.add(*status);
		}
		dumpStatus(*status, NULL, "Added");
	} else {
//					dumpStatus(*status, "Not added: could solve.");
	}
	{
		boost::lock_guard<MutexType> lck(iterMutex_);
		++solved_;
		done_.notify_all();
	}
}

//void runIt(boost::asio::io_service *io) {
//	std::cerr << "Starting thread: " << boost::this_thread::get_id() << std::endl;
//	io->run();
//	std::cerr << "Stopping thread: " << boost::this_thread::get_id() << std::endl;
//}

void BlockListChecker::init(FixedTable::Ptr table)
{
	std::cerr << "Calculating block list..." << std::endl;
	table_ = table;
	blockList_.clear();
	solved_ = 0;
	iters_ = 0;
	State tmp;
	Point p(0, 0);
	for (int n = 2; n <= numStones_; ++n) {
		std::cerr << "Stones = " << n << std::endl;
		solved_ = iters_ = 0;
		initIter(p, n, tmp);
		std::cerr << std::endl;
		boost::thread t(std::bind(&BlockListChecker::progress, this));
		{
			boost::unique_lock<MutexType> lck(iterMutex_);
			while (solved_ < iters_) {
				done_.wait(lck);
			}
		}
		t.interrupt();
		t.join();
		std::cerr << std::endl;
	}
	std::cerr << "Block list calculated. Size = " << blockList_.size() << std::endl;
//	dump_ << "Solved: " << solved_ << "\n---------------------" << std::endl;
	dump_.flush();
}

void BlockListChecker::progress()
{
	while (true) {
//		std::cerr << "bing" << std::endl;
		int iters, solved;
		{
			boost::lock_guard<MutexType> lck(iterMutex_);
			iters = iters_;
			solved = solved_;
		}
//		std::cerr << iters << " " << solved << std::endl;
		if (solved < iters) {
//			std::cerr << "bang" << std::endl;
			float div = iters / 100.0f;
			int n1 = (int)(solved / div);
			int n2 = 100 - n1;
			std::cerr << "[";
			for (int i = 0; i < n1; ++i)
				std::cerr << "#";
			for (int i = 0; i < n2; ++i)
				std::cerr << "-";
			std::cerr << "]\r";
			std::cerr.flush();
			try {
				boost::this_thread::sleep(progressInterval_);
			} catch(boost::thread_interrupted &e) {
				return;
			}
		}
	}
}

bool BlockListChecker::check(const Status& status, const Point& p)
{
	if (table_ != status.tablePtr()) {
		init(status.tablePtr());
	}
	return !blockList_.hasSubStatus(status, p);
}

const char* BlockListChecker::errorMessage()
{
	return "on blocklist";
}
