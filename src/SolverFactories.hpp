#ifndef SOLVERFACTORIES_H_
#define SOLVERFACTORIES_H_

#include "ExpanderFactory.hpp"
#include "Checker.hpp"
#include "PrioNodeQueueFwd.hpp"

class HeurCalculator;
class ComplexChecker;
struct Options;
class Dumper;

std::shared_ptr<PrioNodeQueue> createPrioQueueFromOptions(const Options& opts);
std::shared_ptr<Dumper> createDumperFromOptions(const Options& opts);


class OptionsBasedExpanderFactory {
	const Options& options_;
	const Table& table_;
	bool log_;
public:
	OptionsBasedExpanderFactory(const Options& opts, const Table& table, bool log):
		options_(opts),
		table_(table),
		log_(log)
	{
	}

	OptionsBasedExpanderFactory(const OptionsBasedExpanderFactory&) = delete;
	OptionsBasedExpanderFactory& operator=(const OptionsBasedExpanderFactory&) = delete;

	std::shared_ptr<const HeurCalculator> createAdvancedHeurCalcularor(float heurMultiplier);
	std::shared_ptr<Expander> createExpander(
			std::shared_ptr<const HeurCalculator> calculator,
			ComplexChecker checker,
			bool log,
			std::shared_ptr<const HeurCalculator> experimentalCalculator = std::shared_ptr<const HeurCalculator>());
	std::vector<std::shared_ptr<const Checker>> createBasicCheckers(const std::shared_ptr<const HeurCalculator>& calculator);

	ExpanderFactory factory();
};


#endif /* SOLVERFACTORIES_H_ */
