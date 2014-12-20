#ifndef DUMPERFUNCTIONS_H_
#define DUMPERFUNCTIONS_H_

#include "Array.hpp"
#include "FieldType.hpp"
#include "Status/State.hpp"
#include <iostream>
#include <string>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

class Table;
class Node;

void dumpNode(std::ostream& file, const Table& table, const Node& node,
		std::string title = "", const Array<bool> *highlight = nullptr, int indent = 0);

template<class T>
void dumpArray(std::ostream& file, const Array<T> table,
		const std::string& title = "", int indent = 0) {
	std::string indentString(indent, ' ');
	if (!title.empty()) {
		file << indentString << title << std::endl;
	}
	Array<std::string> txts(table.width(), table.height());
	size_t maxlen = 0;
	for (Point  p: arrayRange(table)) {
		txts[p] = boost::lexical_cast<std::string>(table[p]);
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
void dumpStatus(std::ostream& file, const Status& status, std::string title = "",
		const Array<bool> *highlight = nullptr, int indent = 0)
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
			else if (status.value(p) == FieldType::wall)
				output[p] = '*';
			else if (status.value(p) == FieldType::floor)
			{
				if (highlight != NULL && (*highlight)[p])
					output[p] = '+';
				else
					output[p] = '.';
			} else if (status.value(p) == FieldType::stone)
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
