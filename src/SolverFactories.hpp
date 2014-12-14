#ifndef SOLVERFACTORIES_H_
#define SOLVERFACTORIES_H_

#include "Expander.hpp"
#include "HeurCalculator.hpp"
#include "Dumper/Dumper.hpp"
#include "Options.hpp"
#include "Checker.hpp"
#include "ComplexChecker.hpp"
#include "PrioNodeQueueFwd.hpp"

std::shared_ptr<PrioNodeQueue> createPrioQueueFromOptions(const Options &opts);
Dumper::Ptr createDumperFromOptions(const Options &opts);


class OptionsBasedExpanderFactory {
	const Options& options_;
	const Table& table_;
	bool log_;
public:
	OptionsBasedExpanderFactory(const Options &opts, const Table& table, bool log):
		options_(opts),
		table_(table),
		log_(log)
	{
	}

	OptionsBasedExpanderFactory(const OptionsBasedExpanderFactory&) = delete;
	OptionsBasedExpanderFactory& operator=(const OptionsBasedExpanderFactory&) = delete;

	HeurCalculator::Ptr createAdvancedHeurCalcularor(float heurMultiplier);
	Expander::Ptr createExpander(
			HeurCalculator::Ptr calculator,
			ComplexChecker checker,
			bool log,
			HeurCalculator::Ptr experimentalCalculator = HeurCalculator::Ptr());
	std::vector<Checker::Ptr> createBasicCheckers(const HeurCalculator::Ptr& calculator);

	ExpanderFactory factory();
};


#endif /* SOLVERFACTORIES_H_ */
