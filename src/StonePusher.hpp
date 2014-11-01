#ifndef STONEPUSHER_H_
#define STONEPUSHER_H_

#include "Node.hpp"
#include "HeurCalculator.hpp"
#include "Expander.hpp"
#include "NodeFactory.hpp"
#include <deque>

class State;

class StonePusher: public Expander {
	Expander::Ptr expander_;
	HeurCalculator::Ptr calculator_;
	NodeFactory::Ptr nodeFactory_;
public:
	StonePusher(Expander::Ptr expander, HeurCalculator::Ptr calculator,
			NodeFactory::Ptr nodeFactory);
	virtual bool expand(const Status &status, std::shared_ptr<Node> base,
			NodePusher& queue, Dumper::Ptr dumper);
};



#endif /* STONEPUSHER_H_ */
