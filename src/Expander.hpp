#ifndef EXPANDER_H_
#define EXPANDER_H_

#include <memory>
#include "Dumper/Dumper.hpp"

class Status;
class Node;
class PrioNodeQueue;

class Expander {
public:
	typedef std::shared_ptr<Expander> Ptr;
	// returns true if no more expansions should be made
	virtual void expand(const Status&, std::shared_ptr<Node>, PrioNodeQueue&,
			Dumper::Ptr) = 0;
	virtual ~Expander() {}
};

typedef std::function<Expander::Ptr()> ExpanderFactory;

#endif /* EXPANDER_H_ */
