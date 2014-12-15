#include "Dumper/DumperFunctions.hpp"
#include "Status/Status.hpp"
#include "Status/Table.hpp"
#include "Node.hpp"
#include <boost/format.hpp>

void dumpNode(std::ostream &file, const Table& table, const Node &node,
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

