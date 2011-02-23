#include "Dumper.h"
#include <boost/format.hpp>

void dumpStatus(std::ostream &file, const Status &status,
		std::string title , const Array<bool> *highlight)
{
	title += "\n";
	for (State::const_iterator it = status.state().begin();
			it != status.state().end(); ++it) {
		title += (boost::format("%d,%d ") %
				it->x % it->y).str();
	}
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
				output[p] = '*';
			else if (status.value(p) == ftFloor)
			{
				if (highlight != NULL && (*highlight)[p])
					output[p] = '+';
				else
					output[p] = '.';
			} else if (status.value(p) == ftStone)
			{
				if (highlight != NULL && (*highlight)[p])
					output[p] = 'O';
				else
					output[p] = 'o';
			}
		}
	}
	dumpArray(file, output, title, "");
}


void dumpNode(std::ostream &file, FixedTable::Ptr table, const Node &node,
		std::string title, const Array<bool> *highlight)
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
		highlight2.reset(status.width(), status.height(), false);
	highlight2[node.from() + node.d()] = true;
	dumpStatus(file, status, title, &highlight2);

}

