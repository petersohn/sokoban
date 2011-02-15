#include "Common.h"
#include "Status.h"
#include "Options.h"
#include "Node.h"
#include "Solver.h"
#include "HeurCalculator.h"
#include <time.h>
#include <iostream>
#include <deque>
#include <boost/format.hpp>

using namespace std;


int main(int argc, char** argv) {
	Options opts;
	
	Status st(loadFromFile(opts.filename().c_str()));

	clock_t time0 = clock();
	Solver s();
	std::deque<Node::Ptr> solution = s.solve(st, HeurCalculator::create(), true, opts.enableDump());
	clock_t time = clock() - time0;
	cerr << "Time:" << (double)time/CLOCKS_PER_SEC << endl;
	if (solution.size() == 0)
		cerr << "No solution." << endl;
	else
	{
		for (size_t i = 0; i < solution.size(); i++)
		{
			cout << boost::format("(%2d, %2d) --> %s") %
					solution[i].first.x % solution[i].first.y %
					solution[i].second.x % solution[i].second.y << endl;
		}
	}
}
