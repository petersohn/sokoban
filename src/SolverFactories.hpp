#ifndef SOLVERFACTORIES_H_
#define SOLVERFACTORIES_H_

#include "QueueInterfaces.hpp"
#include "Expander.hpp"
#include "HeurCalculator.hpp"
#include "Dumper/Dumper.hpp"
#include "Options.hpp"
#include "Checker.hpp"

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
