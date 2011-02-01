#include "sokoban.h"
#include <time.h>
#include <iostream>
#include <boost/format.hpp>

using namespace std;

int main(int argc, char** argv) {
	if (argc < 2)
	{
		cerr << "Nincs megadva bemeneti fájl!" << endl;
		return 1;
	}
	Sokoban game(argv[1]);
	
	clock_t time0 = clock();
	vector<pair<Point, Point> > solution = game.solve();
	clock_t time = clock() - time0;
	cerr << "Ido:" << (double)time/CLOCKS_PER_SEC << endl;
	if (solution.size() == 0)
		cerr << "Nincs megoldas." << endl;
	else
	{
		for (size_t i = 0; i < solution.size(); i++)
		{
			cout << boost::format("(%2d, %2d) --> (%2d, %2d)") %
					solution[i].first.x % solution[i].first.y %
					solution[i].second.x % solution[i].second.y << endl;
		}
	}
}
