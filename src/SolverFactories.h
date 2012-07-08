#ifndef SOLVERFACTORIES_H_
#define SOLVERFACTORIES_H_

#include "QueueInterfaces.h"
#include "Expander.h"
#include "HeurCalculator.h"
#include "Dumper/Dumper.h"
#include "Options.h"
#include "Checker.h"

NodeQueue::Ptr createPrioQueueFromOptions(const Options &opts);
Dumper::Ptr createDumperFromOptions(const Options &opts);


class OptionsBasedExpanderFactory {
	const Options &options_;
	FixedTable::Ptr table_;
	bool log_;
public:
	OptionsBasedExpanderFactory(const Options &opts, FixedTable::Ptr table, bool log = true):
		options_(opts),
		table_(table),
		log_(log)
	{
	}

	HeurCalculator::Ptr createAdvancedHeurCalcularor();
	Expander::Ptr createExpander(
			HeurCalculator::Ptr calculator,
			Checker::Ptr checker,
			bool log,
			HeurCalculator::Ptr experimentalCalculator = HeurCalculator::Ptr());
	std::vector<Checker::Ptr> createBasicCheckers(const HeurCalculator::Ptr& calculator);

	ExpanderFactory factory();
};


#endif /* SOLVERFACTORIES_H_ */
