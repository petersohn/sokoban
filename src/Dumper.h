#ifndef DUMPER_H_
#define DUMPER_H_

#include "Common.h"
#include "Array.h"
#include "Table.h"
#include "Status.h"
#include "Node.h"
#include <iostream>
#include <string>
#include <boost/format.hpp>

template<class T>
void dumpArray(std::ostream &file, const Array<T> table,
		const std::string &title = "", const std::string &prefix = "");

void dumpStatus(std::ostream &file, const Status &status,
		std::string title = "", const Array<bool> *highlight = NULL);

void dumpNode(std::ostream &file, FixedTable::Ptr table, const Node &node,
		std::string title = "", const Array<bool> *highlight = NULL);




template<class T>
void dumpArray(std::ostream &file, const Array<T> table,
		const std::string &title, const std::string &prefix) {
	file << title << std::endl;
	Array<std::string> txts(table.width(), table.height());
	Point p;
	size_t maxlen = 0;
	for (p.y = 0; p.y < table.height(); p.y++)
		for (p.x = 0; p.x < table.width(); p.x++) {
			txts[p] = (boost::format("%1%") % table[p]).str();
			maxlen = std::max(maxlen, txts[p].size());
		}
	++maxlen;
	for (p.y = 0; p.y < table.height(); p.y++) {
		for (p.x = 0; p.x < table.width(); p.x++) {
			file.width(maxlen);
			file << txts[p];
		}
		file << std::endl;
	}
	file << std::endl;
}

#endif /* DUMPER_H_ */
