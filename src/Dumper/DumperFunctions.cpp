#include "DistanceCalculator.hpp"
#include "Node.hpp"
#include "Dumper/DumperFunctions.hpp"
#include "Status/Status.hpp"
#include "Status/Table.hpp"
#include <boost/format.hpp>

#include "MatrixIO.hpp"
namespace sokoban {

namespace {

Matrix<bool> getHighlightedPath(const Table& table, const Node& node) {
    Status status{table};
    Point from;
    if (node.ancestor()) {
        status.set(*node.ancestor());
        from = node.ancestor()->from();
    } else {
        status.set(node);
        status.moveStone(node.from() + node.d(), node.from());
        from = table.startingPoint();
    }

    Matrix<int> distances = calculateDistances(table.width(), table.height(),
            from,
            [&](Point p, Point d) {
                return status.value(p+d) == FieldType::floor;
            });
    // TODO: debug it
    // dumpStatus(std::cerr, status);
    // std::cerr << distances;

    Matrix<bool> result{status.width(), status.height(), false};
    auto check = [&](Point& p, Point d) {
        int value = matrixAt(distances, p + d, -1);
        if (value >= 0 && value < distances[p]) {
            p += d;
            return true;
        }
        return false;
    };
    Point p = node.from() - node.d();
    do {
        result[p] = true;
        if (check(p, p10));
        else if (check(p, p01));
        else if (check(p, -p10));
        else if (check(p, -p01));
        else {
            // std::cerr << "foobar\n";
            return result;
        }
    } while (distances[p] != 0);
    // std::cerr << result;
    result[p] = true;
    return result;
}

} // unnamed namespace

void dumpNode(std::ostream& file, const Table& table, const Node& node,
        std::string title, const Matrix<bool> *highlight, int indent) {
    if (title.length() > 0)
        title += ": ";
    title += (boost::format("heur = (%d + %d = %d) d = %d") %
                node.cost() % node.heur() % node.costFgv() %
                node.minDistance()).str();
    Status status(table, node);
    Matrix<bool> highlightedPath = getHighlightedPath(table, node);
    if (highlight != nullptr) {
        for (Point p : matrixRange(highlightedPath)) {
            highlightedPath[p] = highlightedPath[p] && (*highlight)[p];
        }
    }
    highlightedPath[node.from() + node.d()] = true;
    highlightedPath[node.from()] = true;
    dumpStatus(file, status, title, &highlightedPath, indent);
}

} // namespace sokoban
