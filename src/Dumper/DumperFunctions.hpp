#ifndef DUMPERFUNCTIONS_H_
#define DUMPERFUNCTIONS_H_

#include "Matrix.hpp"
#include "FieldType.hpp"
#include "Status/State.hpp"

#include <util/matrix/DumperFunctions.hpp>

#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

#include <iostream>
#include <string>

namespace sokoban {

class Table;
class Node;

void dumpNode(std::ostream& file, const Table& table, const Node& node,
        std::string title = "", const Matrix<bool> *highlight = nullptr,
        int indent = 0);

template <class Status>
void dumpStatus(std::ostream& file, const Status& status,
        std::string title = "", const Matrix<bool> *highlight = nullptr,
        int indent = 0) {
    if (!title.empty()) {
        title += "\n";
        title += std::string(indent, ' ');
    }
    for (State::const_iterator it = status.state().begin();
            it != status.state().end(); ++it) {
        title += (boost::format("%d,%d ") %
                it->x % it->y).str();
    }
    Point p;
    Matrix<char> output(status.width(), status.height());
    for (p.y = 0; p.y < static_cast<int>(status.height()); p.y++)
    {
        for (p.x = 0; p.x < static_cast<int>(status.width()); p.x++)
        {
            if (status.currentPos() == p)
                output[p] = 'Y';
            else if (status.table().destination() == p)
                output[p] = 'X';
            else if (status.value(p) == FieldType::wall)
                output[p] = '*';
            else if (status.value(p) == FieldType::floor)
            {
                if (highlight != nullptr && (*highlight)[p])
                    output[p] = '+';
                else
                    output[p] = '.';
            } else if (status.value(p) == FieldType::stone)
            {
                if (highlight != nullptr && (*highlight)[p])
                    output[p] = 'O';
                else
                    output[p] = 'o';
            }
        }
    }
    dumpMatrix(file, output, title, indent);
}

} // namespace sokoban

#endif /* DUMPERFUNCTIONS_H_ */
