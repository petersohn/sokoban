#include "Common.h"
#include "Array.h"
#include "Status.h"
#include "Node.h"
#include "Table.h"
#include <algorithm>
#include <boost/format.hpp>


using namespace std;

static void floodFillIter(const Status &table, const Point & p, Array<bool> &result,
		deque<int> *border, MinMax *minmax)
{
	if (table.value(p) != ftFloor || result[p])
	{
		if (border != NULL && table.value(p) == ftStone)
			border->push_back(table.stoneAt(p));
		return;
	}
	result[p] = true;
	if (minmax != NULL) {
		minmax->minX = std::min(minmax->minX, p.x);
		minmax->maxX = std::max(minmax->maxX, p.x);
		minmax->minY = std::min(minmax->minY, p.y);
		minmax->maxY = std::max(minmax->maxY, p.y);
	}
	floodFillIter(p+p10, result, border, minmax);
	floodFillIter(p+pm10, result, border, minmax);
	floodFillIter(p+p01, result, border, minmax);
	floodFillIter(p+p0m1, result, border, minmax);
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

static void basicDump(std::ostream &file, const Array<char> table,
		const std::string &title, const std::string &prefix) {
	if (title.length() > 0)
		file << prefix << title << endl;
	Point p;
	for (p.y = 0; p.y < height; p.y++) {
		file << prefix;
		for (p.x = 0; p.x < width; p.x++) {
			file.width(3);
			file << right << table[p];
		}
		file << endl;
	}
	file << endl;
}

void dumpStatus(std::ostream &file, const Status &status,
		std::string title = "", Array<bool> *highlight = NULL)
{
	title += "\n";
	for (int i = 0; i < node.state().size(); i++)
		title += (boost::format("%d,%d ") %
				node.state()[i].x % node.state()[i].y).str();
	Point p;
	Array<char> output(table.width(), table.height());
	for (p.y = 0; p.y < height; p.y++)
	{
		for (p.x = 0; p.x < width; p.x++)
		{
			if (status.currentPos() == p)
				output[p] = 'Y';
			else if (status.table().destination() == p)
				output[p] = 'X';
			else if (status.value(p) == ftWall)
				file << '*';
			else if (status.value(p) == ftFloor)
			{
				if (highlight != NULL && (*highlight)[p])
					file << "+";
				else
					file << '.';
			} else if (status.value(p) == ftStone)
			{
				if (highlight != NULL && (*highlight)[p])
					file << 'O';
				else
					file << 'o';
			}
		}
	}
	basicDump(file, output, title, "");
}


void dumpNode(std::ostream &file, const Table &table, const Node &node,
		std::string title = "", Array<bool> *highlight = NULL)
{
	if (title.length() > 0)
		title += ": ";
	title += (boost::format("(%d + %d = %d)") %
				node.cost() % node.heur() % node.costFgv()).str();
	Status status(table, node);
	Array<bool> highlight2;
	if (highlight != NULL)
		highlight2 = highlight;
	else
		highlight2.reset(status.width(), status.height());
	highlight2[node.state()[node.stone()]] = true;
	dumpStatus(file, status, title, highlight2);

}

Status loadFromFile(const char *filename) {
	using namespace std;

	ifstream file(filename, ifstream::in);
	size_t height, width;
	file >> height >> width;
	string line;
	getline(file, line); // dummy
	Table t(width, height);
	VisitedState vs;
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
				vs.currentPos(p);
				t.wall(p, false);
				startPosOK = true;
				break;
			case '.':
				t.wall(p, false);
				break;
			case 'o':
			case 'O':
				t.wall(p, false);
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
	return Status(FixedTable::Ptr(new FixedTable(t)), vs);
}



