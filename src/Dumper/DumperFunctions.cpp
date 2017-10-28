#include "Dumper/DumperFunctions.hpp"
#include "Status/Status.hpp"
#include "Status/Table.hpp"
#include "Node.hpp"
#include <boost/format.hpp>

namespace sokoban {

void dumpNode(std::ostream& file, const Table& table, const Node& node,
        std::string title, const Matrix<bool> *highlight, int indent) {
    if (title.length() > 0)
        title += ": ";
    title += (boost::format("heur = (%d + %d = %d) d = %d") %
                node.cost() % node.heur() % node.costFgv() %
                node.minDistance()).str();
    Status status(table, node);
    Matrix<bool> highlight2;
    if (highlight != nullptr)
        highlight2 = *highlight;
    else
        highlight2.reset(status.width(), status.height(), false);
    highlight2[node.from() + node.d()] = true;
    dumpStatus(file, status, title, &highlight2, indent);
}

} // namespace sokoban
