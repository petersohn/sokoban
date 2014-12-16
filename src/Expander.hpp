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
			std::shared_ptr<Dumper>) = 0;
	virtual ~Expander() {}
};


#endif /* EXPANDER_H_ */
