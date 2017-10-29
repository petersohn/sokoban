#ifndef DUMPERFUNCTIONS_H_
#define DUMPERFUNCTIONS_H_

#include "Matrix.hpp"
#include "FieldType.hpp"
#include "Status/State.hpp"

#include <util/matrix/DumperFunctions.hpp>

#include <boost/format.hpp>
#include <boost/variant.hpp>

#include <iostream>
#include <string>

namespace sokoban {

class Table;
class Node;

using Highlight = boost::variant<bool, char>;

namespace detail {

template<typename Status>
class HighlightVisitor : public boost::static_visitor<char> {
public:
    HighlightVisitor(const Status& status, Point p) : status(status), p(p) {}

    char operator()(bool value) {
        if (status.currentPos() == p) {
            return 'Y';
        } else if (status.table().destination() == p) {
            return 'X';
        } else if (status.value(p) == FieldType::wall) {
            return '*';
        } else if (status.value(p) == FieldType::floor) {
            return value ? '+' : '.';
        } else if (status.value(p) == FieldType::stone) {
            return value ? 'O' : 'o';
        }
        return ' ';
    }

    char operator()(char value) {
        return value;
    }

private:
    const Status& status;
    Point p;
};

} // namespace detail

template<typename Status>
char getHighlight(const Status& status, Point p, const Highlight& highlight) {
    detail::HighlightVisitor<Status> visitor{status, p};
    return boost::apply_visitor(visitor, highlight);
}

void dumpNode(std::ostream& file, const Table& table, const Node& node,
        std::string title = "",
        boost::optional<Matrix<Highlight>> highlight = boost::none,
        int indent = 0);

template <class Status>
void dumpStatus(std::ostream& file, const Status& status,
        std::string title = "",
        const boost::optional<Matrix<Highlight>>& highlight = boost::none,
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
        for (p.x = 0; p.x < static_cast<int>(status.width()); p.x++) {
            output[p] = getHighlight(status, p,
                    highlight ? (*highlight)[p] : false);
        }
    }
    dumpMatrix(file, output, title, indent);
}

} // namespace sokoban

#endif /* DUMPERFUNCTIONS_H_ */
