#ifndef SOLVERFACTORIES_H_
#define SOLVERFACTORIES_H_

#include "QueueInterfaces.h"
#include "Expander.h"
#include "HeurCalculator.h"

NodeQueue::Ptr createPrioQueue();
Expander::Ptr createExpander();
Expander::Ptr createExpanderWithCalculator(HeurCalculator::Ptr calc);


#endif /* SOLVERFACTORIES_H_ */
