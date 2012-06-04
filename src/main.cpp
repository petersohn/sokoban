#include "Common.h"
#include "Status.h"
#include "Options.h"
#include "Node.h"
#include "Solver.h"
#include "SolverFactories.h"
#include "DumperFunctions.h"
#include "State.h"
#include "ThreadPool.h"
#include "SolutionChecker.h"
#include <time.h>
#include <iostream>
#include <fstream>
#include <deque>
#include <boost/format.hpp>
#include <functional>
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace std;
namespace pt = boost::posix_time;

int main(int argc, char** argv) {
	Options opts(argc, argv, "sokoban.cfg");

	Status st(Status::loadFromFile(opts.filename().c_str()));

	cerr << "Number of threads: " << opts.numThreads() << endl;
	ThreadPool::instance()->numThreads(opts.numThreads());
	ThreadPool::instance()->start();
	clock_t processorTime0 = clock();
	pt::ptime realTime0 = pt::microsec_clock::local_time();
	Solver s(std::bind(createPrioQueueFromOptions, opts),
			std::bind(createExpanderFromOptions, opts, st.tablePtr(), true),
			std::bind(createDumperFromOptions, opts),
			opts.parallelOuterExpand());
	std::deque<Node::Ptr> solution = s.solve(st);
	clock_t processorTime = clock() - processorTime0;
	pt::time_duration realTime =  pt::microsec_clock::local_time() - realTime0;
	cerr << "Length of solution: " << solution.size() << endl;
	cerr << "Processor Time:" << (double)processorTime/CLOCKS_PER_SEC << endl;
	cerr << "Real Time:" << pt::to_simple_string(realTime) << endl;
	ThreadPool::instance()->wait();
	if (solution.size() == 0)
		cerr << "No solution." << endl;
	else
	{
		if (checkResult(st, solution)) {
			cerr << "Solution OK." << endl;
		} else {
			cerr << "Solution bad." << endl;
		}
		std::ofstream dump("solution.dump", std::ios::out | std::ios::trunc);
		for (std::deque<Node::Ptr>::iterator it = solution.begin();
				it != solution.end(); ++it)
		{
			dumpNode(dump, st.tablePtr(), **it);
			Point from = (*it)->from();
			Point p(from - (*it)->d());
			std::string dir =
					! opts.oldStyleOutput() ? direction((*it)->d()) :
						(boost::format("(%2d, %2d)") %
						p.x % p.y).str();
			cout << boost::format("(%2d, %2d) --> %s") %
					from.x % from.y % dir << endl;
		}
	}
	cerr << "Status copied " << Status::copyCount << " times." << endl;
}
