#ifndef DUMPERFUNCTIONS_H_
#define DUMPERFUNCTIONS_H_

#include "Common.hpp"
#include "Array.hpp"
#include "Status/Table.hpp"
#include "Node.hpp"
#include <iostream>
#include <string>
#include <boost/format.hpp>

template<class T>
void dumpArray(std::ostream &file, const Array<T> table,
		const std::string &title = "", int indent = 0);

template <class Status>
void dumpStatus(std::ostream &file, const Status &status,
		std::string title = "", const Array<bool> *highlight = nullptr, int indent = 0);

void dumpNode(std::ostream &file, FixedTable::Ptr table, const Node &node,
		std::string title = "", const Array<bool> *highlight = nullptr, int indent = 0);




template<class T>
void dumpArray(std::ostream &file, const Array<T> table,
		const std::string &title, int indent) {
	std::string indentString(indent, ' ');
	if (!title.empty()) {
		file << indentString << title << std::endl;
	}
	Array<std::string> txts(table.width(), table.height());
	size_t maxlen = 0;
	for (Point  p: arrayRange(table)) {
		txts[p] = (boost::format("%1%") % table[p]).str();
		maxlen = std::max(maxlen, txts[p].size());
	}
	// leave a space between characters
	++maxlen;
	Point p;
	for (p.y = 0; p.y < static_cast<int>(table.height()); p.y++) {
		file << indentString;
		for (p.x = 0; p.x < static_cast<int>(table.width()); p.x++) {
			file.width(maxlen);
			file << txts[p];
		}
		file << std::endl;
	}
	file << std::endl;
}

template <class Status>
void dumpStatus(std::ostream &file, const Status &status,
		std::string title , const Array<bool> *highlight, int indent)
{
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
	Array<char> output(status.width(), status.height());
	for (p.y = 0; p.y < static_cast<int>(status.height()); p.y++)
	{
		for (p.x = 0; p.x < static_cast<int>(status.width()); p.x++)
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
	dumpArray(file, output, title, indent);
}




#endif /* DUMPERFUNCTIONS_H_ */
