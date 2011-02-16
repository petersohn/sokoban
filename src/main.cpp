#include "Common.h"
#include "Status.h"
#include "Options.h"
#include "Node.h"
#include "Solver.h"
#include "HeurCalculator.h"
#include "Dumper.h"
#include <time.h>
#include <iostream>
#include <fstream>
#include <deque>
#include <boost/format.hpp>

using namespace std;


int main(int argc, char** argv) {
	Options opts(argc, argv);
	
	Status st(Status::loadFromFile(opts.filename().c_str()));

	clock_t time0 = clock();
	Solver s;
	std::deque<Node> solution = s.solve(st, HeurCalculator::create(), true, opts.enableDump());
	clock_t time = clock() - time0;
	cerr << "Time:" << (double)time/CLOCKS_PER_SEC << endl;
	if (solution.size() == 0)
		cerr << "No solution." << endl;
	else
	{
		std::ofstream dump("solution.dump", std::ios::out | std::ios::trunc);
		for (std::deque<Node>::iterator it = solution.begin();
				it != solution.end(); ++it)
		{
			dumpNode(dump, st.tablePtr(), *it);
			Point from(it->state()[it->stone()] - it->d());
			std::string dir =
					it->d().x > 0 ? "right" :
					it->d().x < 0 ? "left" :
					it->d().y > 0 ? "down" :
					it->d().y < 0 ? "up" : "???";
			cout << boost::format("(%2d, %2d) --> %s") %
					from.x % from.y % dir << endl;
		}
	}
}
