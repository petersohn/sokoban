#ifndef EXPANDER_H_
#define EXPANDER_H_

#include <boost/shared_ptr.hpp>
#include "QueueInterfaces.h"

class Status;
class Node;

class Expander {
public:
	typedef boost::shared_ptr<Expander> Ptr;
	// returns true if no more expansions should be made
	virtual bool expand(const Status &, boost::shared_ptr<Node>, NodePusher &) = 0;
	virtual ~Expander() {}
};

#endif /* EXPANDER_H_ */
