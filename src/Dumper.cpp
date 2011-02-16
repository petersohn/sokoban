#include "Dumper.h"
#include <boost/format.hpp>

static void basicDump(std::ostream &file, const Array<char> table,
		const std::string &title, const std::string &prefix) {
	if (title.length() > 0)
		file << prefix << title << std::endl;
	Point p;
	for (p.y = 0; p.y < table.height(); p.y++) {
		file << prefix;
		for (p.x = 0; p.x < table.width(); p.x++) {
			file.width(3);
			file << std::right << table[p];
		}
		file << std::endl;
	}
	file << std::endl;
}

void dumpStatus(std::ostream &file, const Status &status,
		std::string title , const Array<bool> *highlight)
{
	title += "\n";
	for (int i = 0; i < status.state().size(); i++)
		title += (boost::format("%d,%d ") %
				status.state()[i].x % status.state()[i].y).str();
	Point p;
	Array<char> output(status.width(), status.height());
	for (p.y = 0; p.y < status.height(); p.y++)
	{
		for (p.x = 0; p.x < status.width(); p.x++)
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


void dumpNode(std::ostream &file, FixedTable::Ptr table, const Node &node,
		std::string title = "", Array<bool> *highlight = NULL)
{
	if (title.length() > 0)
		title += ": ";
	title += (boost::format("(%d + %d = %d)") %
				node.cost() % node.heur() % node.costFgv()).str();
	Status status(table, node);
	Array<bool> highlight2;
	if (highlight != NULL)
		highlight2 = *highlight;
	else
		highlight2.reset(status.width(), status.height());
	highlight2[node.state()[node.stone()]] = true;
	dumpStatus(file, status, title, &highlight2);

}

