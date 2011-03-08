#ifndef SOLVER_H_
#define SOLVER_H_

#include "QueueInterfaces.h"
#include "Expander.h"
#include "Dumper.h"
#include <deque>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

class Status;
class Node;

class Solver {
public:
	typedef boost::shared_ptr<Solver> Ptr;
	typedef boost::function<NodeQueue::Ptr()> QueueFactory;
	typedef boost::function<Expander::Ptr()> ExpanderFactory;
private:
	QueueFactory queueFactory_;
	ExpanderFactory expanderFactory_;
	Dumper::Ptr dumper_;
public:
	Solver(QueueFactory qf, ExpanderFactory ef, Dumper::Ptr dumper):
		queueFactory_(qf),
		expanderFactory_(ef),
		dumper_(dumper)
	{}
	std::deque<boost::shared_ptr<Node> > solve(Status status);
	virtual ~Solver() {}
private:
};

#endif /* SOLVER_H_ */
