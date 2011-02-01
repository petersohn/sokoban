#include "Problem.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <exception>


using namespace std;

Problem::Problem():width_(0), height_(0), stoneNum_(0) {
}

void Problem::reload(const char *filename) {
	ifstream file(fileName, ifstream::in);
	size_t height, width;
	file >> height >> width;
	string line;
	getline(file, line); // dummy
	table_.reset(width, height, 0);
	stoneNum_ = 0;
	table_.zero();

	bool startPosOK = false, destinationOK = false;
	Point p;
	Point startPos;
	while (file.good())
	{
		getline(file, line);
		for (p.x = 0; p.x < line.length() && p.x < width; ++p.x)
		{
			cerr.width(3);
			cerr << left << line[p.x];
			switch (line[p.x])
			{
			case 'X':
			case 'x':
				destination_ = p;
				table_[p] = F_FLOOR;
				destinationOK = true;
				break;
			case 'Y':
			case 'y':
				startPos = p;
				table_[p] = F_FLOOR;
				startPosOK = true;
				break;
			case '.':
				startField[p] = F_FLOOR;
				break;
			case 'o':
			case 'O':
				table_[p] = F_STONE;
				++stoneNum_;
				break;
			default:
				table_[p] = F_WALL;
			}
		}
		cerr << endl;
		if (++p.y >= height)
			break;
	}
	cerr << endl;
	if (stoneNum == 0 || !startPosOK || !destinationOK)
		throw exception(); // TODO exception handling
	initState();
	beginningState_.currentPos_ = startPos;
}

void Problem::initState() {
	vector<Point> newStones;
	newStones.reserve(stoneNum());
	Point p;
	for (p.y = 0; p.y < height; ++p.y)
		for (p.x = 0; p.x < width; ++p.x)
		{
			if (table(p) == F_STONE)
			{
				newStones.push_back(p);
			}
		}
	beginningState_.setStones(newStones);
	stones.currentPos = Point(startPos);
}
