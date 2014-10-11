#ifndef SOLVER_H_
#define SOLVER_H_

#include "QueueInterfaces.hpp"
#include "Expander.hpp"
#include "Dumper/Dumper.hpp"
#include "util/ThreadPool.hpp"
#include <deque>
#include <functional>
#include <memory>

class Status;
class Node;

class NullDumperFactory {
public:
	Dumper::Ptr operator()() { return Dumper::Ptr(); }
};

class Solver {
public:
	typedef std::unique_ptr<Solver> Ptr;
private:
	QueueFactory queueFactory_;
	ExpanderFactory expanderFactory_;
	DumperFactory dumperFactory_;
	std::size_t numThreads_;
	util::ThreadPool threadPool_;
public:
	Solver(QueueFactory qf, ExpanderFactory ef, DumperFactory df = NullDumperFactory(),
			std::size_t numThreads = 0):
		queueFactory_(qf),
		expanderFactory_(ef),
		dumperFactory_(df),
		numThreads_(numThreads)
	{
		if (numThreads > 0) {
			threadPool_.setNumThreads(numThreads);
		}
	}
	std::deque<std::shared_ptr<Node>> solve(Status status);
	~Solver() {}
private:
};

#endif /* SOLVER_H_ */
