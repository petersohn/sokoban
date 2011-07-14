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

class NullDumperFactory {
public:
	Dumper::Ptr operator()() { return Dumper::Ptr(); }
};

class Solver {
public:
	typedef boost::shared_ptr<Solver> Ptr;
	typedef boost::function<NodeQueue::Ptr()> QueueFactory;
	typedef boost::function<Expander::Ptr()> ExpanderFactory;
	typedef boost::function<Dumper::Ptr()> DumperFactory;
private:
	QueueFactory queueFactory_;
	ExpanderFactory expanderFactory_;
	DumperFactory dumperFactory_;
public:
	Solver(QueueFactory qf, ExpanderFactory ef, DumperFactory df = NullDumperFactory()):
		queueFactory_(qf),
		expanderFactory_(ef),
		dumperFactory_(df)
	{}
	std::deque<boost::shared_ptr<Node> > solve(const Status &status);
	~Solver() {}
private:
};

#endif /* SOLVER_H_ */
