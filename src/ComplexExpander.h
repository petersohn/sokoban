#ifndef COMPLEXEXPANDER_H_
#define COMPLEXEXPANDER_H_

#include "Expander.h"
#include "Node.h"
#include <vector>

class ComplexExpander: public Expander {
	typedef std::vector<Expander::Ptr> ContainerType;
	ContainerType funcs_;
public:
	template<class Iterator>
	ComplexExpander(Iterator first, Iterator last):
		funcs_(first, last)
	{}
	virtual bool expand(const Status &status, std::shared_ptr<Node> base,
			NodePusher& queue, Dumper::Ptr dumper);
};

#endif /* COMPLEXEXPANDER_H_ */
