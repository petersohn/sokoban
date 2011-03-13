#ifndef SOLVERFACTORIES_H_
#define SOLVERFACTORIES_H_

#include "QueueInterfaces.h"
#include "Expander.h"
#include "HeurCalculator.h"
#include "Dumper.h"
#include "Options.h"

NodeQueue::Ptr createPrioQueue();
Expander::Ptr createExpander(bool log = false);
Expander::Ptr createExpanderWithCalculator(HeurCalculator::Ptr calc, bool log = false);
Expander::Ptr createExpanderFromOptions(const Options &opts, bool log = true);
Dumper::Ptr createDumperFromOptions(const Options &opts);


#endif /* SOLVERFACTORIES_H_ */
