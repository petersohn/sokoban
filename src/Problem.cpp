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
	table_.reset(width, height, ftFloor);
	bareTable_.reset(width, height, ftFloor);
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
				setTable(p, ftFloor);
				destinationOK = true;
				break;
			case 'Y':
			case 'y':
				startPos = p;
				setTable(p, ftFloor);
				startPosOK = true;
				break;
			case '.':
				setTable(p, ftFloor);
				break;
			case 'o':
			case 'O':
				setTable(p, ftStone);
				break;
			default:
				setTable(p, ftWall);
			}
		}
		cerr << endl;
		if (++p.y >= height)
			break;
	}
	cerr << endl;
	if (stoneNum == 0 || !startPosOK || !destinationOK)
		throw exception(); // TODO exception handling
	stateOK_ = false;
}

void Problem::initState() {
	vector<Point> newStones;
	newStones.reserve(stoneNum());
	Point p;
	for (p.y = 0; p.y < height(); ++p.y)
		for (p.x = 0; p.x < width(); ++p.x)
		{
			if (table(p) == F_STONE)
			{
				newStones.push_back(p);
			}
		}
	beginningState_.setStones(newStones);
	beginningState_.currentPos = Point(startPos);
}

void Problem::clearStones() {
	Point p;
	for (p.y = 0; p.y < height(); ++p.y)
		for (p.x = 0; p.x < width(); ++p.x) {
			table_[p] = bareTable(p);
		}
	stoneNum_ = 0;
	stateOK_ = false;
}

void Problem::setTable(const Point &p, FieldType f) {
	if (table[p] == f)
		return;
	if (f == ftStone)
		++stoneNum_;
	else
	if (table_[p] == ftStone)
		--stoneNum_;
	table_[p] = f;
	stateOK_ = false;
}

bool Problem::addStone(const Point &p) {
	if (table(p) == ftFloor) {
		table_[p] = ftStone;
		++stoneNum_;
		stateOK_ = false;
		return true;
	}
	return false;
}

bool Problem::removeStone(const Point &p) {
	if (table(p) == ftStone) {
		table_[p] = ftFloor;
		--stoneNum_;
		stateOK_ = false;
		return true;
	}
	return false;
}
