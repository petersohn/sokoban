#include "Common.h"
#include "Status.h"
#include "Options.h"
#include "Node.h"
#include "Solver.h"
#include "SolverFactories.h"
#include "DumperFunctions.h"
#include "State.h"
#include "ThreadPool.h"
#include <time.h>
#include <iostream>
#include <fstream>
#include <deque>
#include <boost/format.hpp>
#include <boost/bind.hpp>

using namespace std;


int main(int argc, char** argv) {
	Options opts(argc, argv, "sokoban.cfg");
//	cout << "Filename: " << opts.filename() << endl;
//	cout << "Dump style: " << opts.dumpStyle() << endl;
//	cout << "Use stone pusher: " << opts.useStonePusher() << endl;
//	cout << "Use movable checker: " << opts.useMovableChecker() << endl;
//	cout << "Use corridor checker: " << opts.useCorridorChecker() << endl;

	Status st(Status::loadFromFile(opts.filename().c_str()));

	ThreadPool::instance()->numThreads(opts.numThreads());
	ThreadPool::instance()->start();
	clock_t time0 = clock();
	Solver s(boost::bind(createPrioQueueFromOptions, opts),
			boost::bind(createExpanderFromOptions, opts, true),
			boost::bind(createDumperFromOptions, opts));
	std::deque<Node::Ptr> solution = s.solve(st);
	clock_t time = clock() - time0;
	cerr << "Length of solution: " << solution.size() << endl;
	cerr << "Time:" << (double)time/CLOCKS_PER_SEC << endl;
	ThreadPool::instance()->wait();
	if (solution.size() == 0)
		cerr << "No solution." << endl;
	else
	{
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
