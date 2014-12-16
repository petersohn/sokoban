#ifndef EXPANDER_H_
#define EXPANDER_H_

#include <memory>
#include "Dumper/Dumper.hpp"
#include "PrioNodeQueueFwd.hpp"

class Status;
class Node;

class Expander {
public:
	// returns true if no more expansions should be made
	virtual void expand(const Status&, std::shared_ptr<Node>, PrioNodeQueue&,
			Dumper::Ptr) = 0;
	virtual ~Expander() {}
};

typedef std::function<std::shared_ptr<Expander>()> ExpanderFactory;

#endif /* EXPANDER_H_ */
