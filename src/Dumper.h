#ifndef DUMPER_H_
#define DUMPER_H_

#include "Table.h"

class Status;
class Node;

class Dumper {

public:
	virtual void dumpStatus(const Status& status, const char *title) = 0;
	virtual void dumpNode(FixedTable::Ptr table, const Node &node, const char *title) = 0;
	virtual ~Dumper() {}
};

#endif /* DUMPER_H_ */
