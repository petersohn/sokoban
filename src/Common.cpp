#include "Common.h"
#include "Array.h"
#include "Status.h"
#include "Node.h"
#include "Table.h"
#include <algorithm>


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

void dumpStatus(std::ostream &file, const Status &status,
		const std::string &title = "", Array<bool> *highlight = NULL)
{
	string prefix = /*(node->bad) ? "! " : */"";
	if (title.length() > 0)
		file << prefix << title << ": (" << node.cost() << " + " << node.heur()
			<< " = " << node.costFgv() << ")" << endl;
	file << prefix;
	for (int i = 0; i < node.state().size(); i++)
		file << node.state()[i].x << "," << node.state()[i].y << " ";
	file << "(" << node->heur() << ")" << endl;
	Point p;
	for (p.y = 0; p.y < height; p.y++)
	{
		file << prefix;
		for (p.x = 0; p.x < width; p.x++)
		{
			file.width(3);
			file << right;
			if (stones.currentPos == p)
				file << 'Y';
			else if (destination == p)
				file << 'X';
			else if (fieldAt(p) == F_WALL)
				file << '*';
			else if (fieldAt(p) == F_FLOOR)
			{
				if (highlight != NULL && (*highlight)[p])
					file << "+";
				else
					file << '.';
			} else if (fieldAt(p) == F_STONE)
			{
				if (node.get() != NULL && stoneAt[p] == node->stone)
					file << 'O';
				else
					file << 'o';
			}
		}
		file << endl;
	}
	file << endl;
}
