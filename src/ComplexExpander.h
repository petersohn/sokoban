#ifndef COMPLEXEXPANDER_H_
#define COMPLEXEXPANDER_H_

#include "Expander.h"
#include "ComplexStrategy.h"
#include "Node.h"
#include <boost/lambda/lambda.hpp>

class ComplexExpander: public Expander {
	struct ExpandParams {
		const Status &status;
		Node::Ptr base;
		NodePusher &queue;
		ExpandParams(const Status &status, boost::shared_ptr<Node> base, NodePusher& queue):
			status(status),
			base(base),
			queue(queue) {}
	};

	ComplexStrategy<ExpandParams&, bool, false> funcs_;
public:
	template<class Iterator>
	ComplexExpander(Iterator first, Iterator last):
		funcs_(first, last,
					boost::lambda::_2,
				boost::lambda::_1)
	{}
	virtual bool expand(const Status &status, boost::shared_ptr<Node> base, NodePusher& queue);
};

#endif /* COMPLEXEXPANDER_H_ */
