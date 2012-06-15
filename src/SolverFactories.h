#ifndef SOLVERFACTORIES_H_
#define SOLVERFACTORIES_H_

#include "QueueInterfaces.h"
#include "Expander.h"
#include "HeurCalculator.h"
#include "Dumper.h"
#include "Options.h"
#include "Checker.h"

NodeQueue::Ptr createPrioQueueFromOptions(const Options &opts);
Dumper::Ptr createDumperFromOptions(const Options &opts);

class CreateExpanderFromOptions {
	const Options &options_;
	FixedTable::Ptr table_;
	bool log_;
public:
	CreateExpanderFromOptions(const Options &opts, FixedTable::Ptr table, bool log = true):
		options_(opts),
		table_(table),
		log_(log)
	{
	}

	HeurCalculator::Ptr createAdvancedHeurCalcularor();
	Expander::Ptr createExpander(HeurCalculator::Ptr calculator, Checker::Ptr checker, bool log);
	std::vector<Checker::Ptr> createBasicCheckers(const HeurCalculator::Ptr& calculator);

	Expander::Ptr operator()();
};


#endif /* SOLVERFACTORIES_H_ */
