#ifndef DUMPER_H_
#define DUMPER_H_

#include "Common.h"
#include "Array.h"
#include "Table.h"
#include "Status.h"
#include "Node.h"
#include <iostream>

void dumpStatus(std::ostream &file, const Status &status,
		std::string title = "", const Array<bool> *highlight = NULL);

void dumpNode(std::ostream &file, FixedTable::Ptr table, const Node &node,
		std::string title = "", const Array<bool> *highlight = NULL);

#endif /* DUMPER_H_ */
