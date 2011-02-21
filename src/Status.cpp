#include "Status.h"
#include <fstream>


Status::Status(FixedTable::Ptr table):
	table_(table),
	state_(),
	stoneAt_(table->get().width(), table->get().height()),
	fields_(table->get().width(), table->get().height()),
	reachOK_(false)
{
	init();
}


Status::Status(FixedTable::Ptr table, const State &state):
	table_(table),
	state_(state),
	stoneAt_(table->get().width(), table->get().height()),
	fields_(table->get().width(), table->get().height()),
	reachOK_(false)
{
	init();
}

Status::Status(FixedTable::Ptr table, const Node &node):
		table_(table),
		state_(node.state()),
		currentPos_(node.currentPos()),
		stoneAt_(table->get().width(), table->get().height()),
		fields_(table->get().width(), table->get().height()),
		reachOK_(false)
{
	init();
}

void Status::init() {
	Point p;
	for (p.y = 0; p.y < table().height(); ++p.y)
		for (p.x = 0; p.x < table().width(); ++p.x)  {
			fields_[p] = table().wall(p) ? ftWall : ftFloor;
		}
	for (int i = 0; i < state_.size(); ++i) {
		if (state_[i] != table().destination()) {
			fields_[state_[i]] = ftStone;
			stoneAt_[state_[i]] = i;
		}
	}
}

void initStones() {

}

void Status::calculateReachable() const {
	reachable_.reset(width(), height(), false);
	floodFill(*this, state().currentPos(), reachable_, &border_);
	reachOK_ = true;
}

bool Status::addStone(const Point &p) {
	if (value(p) != ftFloor)
		return false;
	fields_[p] = ftStone;
	stoneAt_[p] = state_.addStone(p);
	reachOK_ = false;
	return true;
}

bool Status::removeStone(const Point &p) {
	if (value(p) != ftStone)
		return false;
	fields_[p] = ftFloor;
	state_.removeStone(stoneAt(p));
	reachOK_ = false;
	return true;
}

void Status::state(const State &value) {
	state_ = value;
	reachOK_ = false;
	init();
}

bool Status::currentPos(const Point & p) {
	currentPos_ = p;
	reachOK_ = false;
	return true;
}

bool Status::moveStone(int stone, const Point & p) {
	if (value(state()[stone]) != ftStone && value(p) != ftFloor)
		return false;
	fields_[state_[stone]] = ftFloor;
	currentPos_ = state_[stone];
	state_.moveStone(stone, p);
	if (p != table().destination())
	{
		fields_[p] = ftStone;
		stoneAt_[p] = stone;
	} else {
//		state_.removeStone(stone);
	}
	reachOK_ = false;
	return true;
}

void Status::set(const Node &node) {
	state_ = node.state();
	currentPos_ = node.currentPos();
	reachOK = false;
	init();
}


Status Status::loadFromFile(const char *filename) {
	using namespace std;
	ifstream file(filename, ifstream::in);
	size_t height, width;
	file >> height >> width;
	string line;
	getline(file, line); // dummy
	Table t(width, height);
	State st;
	Point startPos;
	bool startPosOK = false, destinationOK = false;
	int stoneNum = 0;
	Point p;
	while (file.good())
	{
		getline(file, line);
		for (p.x = 0; p.x < line.length() && p.x < width; p.x++)
		{
			cerr.width(3);
			cerr << left << line[p.x];
			switch (line[p.x])
			{
			case 'X':
			case 'x':
				t.destination(p);
				t.wall(p, false);
				destinationOK = true;
				break;
			case 'Y':
			case 'y':
				startPos = p;
				t.wall(p, false);
				startPosOK = true;
				break;
			case '.':
				t.wall(p, false);
				break;
			case 'o':
			case 'O':
				t.wall(p, false);
				st.addStone(p);
				++stoneNum;
				break;
			default:
				t.wall(p, true);
			}
		}
		cerr << endl;
		if (++p.y >= height)
			break;
	}
	cerr << endl;
	if (stoneNum == 0 || !startPosOK || !destinationOK)
		throw SokobanException();
	Status result(FixedTable::Ptr(new FixedTable(t)), st);
	result.currentPos(startPos);
	return result;
}



static void floodFillIter(const Status &status, const Point & p, Array<bool> &result,
		std::deque<int> *border, MinMax *minmax)
{
	if (status.value(p) != ftFloor || result[p])
	{
		if (border != NULL && status.value(p) == ftStone)
			border->push_back(status.stoneAt(p));
		return;
	}
	result[p] = true;
	if (minmax != NULL) {
		minmax->minX = std::min(minmax->minX, p.x);
		minmax->maxX = std::max(minmax->maxX, p.x);
		minmax->minY = std::min(minmax->minY, p.y);
		minmax->maxY = std::max(minmax->maxY, p.y);
	}
	floodFillIter(status, p+Point::p10, result, border, minmax);
	floodFillIter(status, p+Point::pm10, result, border, minmax);
	floodFillIter(status, p+Point::p01, result, border, minmax);
	floodFillIter(status, p+Point::p0m1, result, border, minmax);
}

void floodFill(const Status &table, const Point &p0, Array<bool> &result,
			std::deque<int> *border, MinMax *minmax) {
	result.fill(false);
	if (minmax != NULL) {
		minmax->minX = table.width();
		minmax->maxX = 0;
		minmax->minY = table.height();
		minmax->maxY = 0;
	}
	floodFillIter(table, p0, result, border, minmax);
}


inline bool shiftIter(Status &status, const Point &p) {
	if (status.value(p) == ftFloor && status.reachable(p)) {
		status.currentPos(p);
		return true;
	}
	return false;
}

void shiftCurrentPos(Status &status) {
	Point p = status.currentPos();
	if (shiftIter(status, p+Point::p01))
		return;
	if (shiftIter(status, p+Point::p0m1))
		return;
	if (shiftIter(status, p+Point::p10))
		return;
	if (shiftIter(status, p+Point::pm10))
		return;
}

