#include "Problem.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <exception>


using namespace std;

Problem::Problem():width_(0), height_(0), stoneNum_(0), stateOK_(false) {
}

void Problem::reload(const char *filename) {
	ifstream file(fileName, ifstream::in);
	size_t height, width;
	file >> height >> width;
	string line;
	getline(file, line); // dummy

	bool startPosOK = false, destinationOK = false;
	int stoneNum = 0;
	Point p;
	Point startPos;
	Array<bool> stones(width, height);
	Table table(width, height);
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
				table.destination(p);
				stones[p] = false;
				table.setWall(p, false);
				destinationOK = true;
				break;
			case 'Y':
			case 'y':
				startPos = p;
				stones[p] = false;
				table.setWall(p, false);
				startPosOK = true;
				break;
			case '.':
				stones[p] = false;
				table.setWall(p, false);
				break;
			case 'o':
			case 'O':
				stones[p] = true;
				table.setWall(p, false);
				++stoneNum;
				break;
			default:
				stones[p] = false;
				table.setWall(p, true);
			}
		}
		cerr << endl;
		if (++p.y >= height)
			break;
	}
	cerr << endl;
	if (stoneNum == 0 || !startPosOK || !destinationOK)
		throw SokobanException(); // TODO exception handling
	table_.reset(new FixedTable(table));
	stoneNum_ = stoneNum;
	stones_ = stones;
	startPos_ = startPos;
	stateOK_ = false;
}

void Problem::initState() {
	vector<Point> newStones;
	newStones.reserve(stoneNum());
	Point p;
	for (p.y = 0; p.y < height(); ++p.y)
		for (p.x = 0; p.x < width(); ++p.x)
		{
			if (value(p) == ftStone)
			{
				newStones.push_back(p);
			}
		}
	beginningState_.setStones(newStones);
	beginningState_.currentPos(startPos_);
}

void Problem::reset(FixedTable::Ptr table) {
	table_ = table;
	clearStones();
}

void Problem::clearStones() {
	Point p;
	for (p.y = 0; p.y < height(); ++p.y)
		for (p.x = 0; p.x < width(); ++p.x) {
			stones_[p] = false;
		}
	stoneNum_ = 0;
	stateOK_ = false;
}

bool Problem::addStone(const Point &p) {
	if (value(p) == ftFloor) {
		table_[p] = ftStone;
		++stoneNum_;
		stateOK_ = false;
		return true;
	}
	return false;
}

void Problem::startPos(const Point & p)
{
	startPos_ = p;
	beginningState_.currentPos(startPos_);
}

bool Problem::removeStone(const Point &p) {
	if (value(p) == ftStone) {
		table_[p] = ftFloor;
		--stoneNum_;
		stateOK_ = false;
		return true;
	}
	return false;
}
