#include "Dumper/DumperFunctions.h"
#include "Status/Status.h"
#include <boost/format.hpp>

void dumpNode(std::ostream &file, FixedTable::Ptr table, const Node &node,
		std::string title, const Array<bool> *highlight, int indent)
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
	dumpStatus(file, status, title, &highlight2, indent);

}

