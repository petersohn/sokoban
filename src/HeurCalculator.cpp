#include "HeurCalculator.h"
#include "BasicHeurCalculator.h"
#include "AdvancedHeurCalculator.h"

HeurCalculator::Ptr HeurCalculator::create() {
	return Ptr(new AdvancedHeurCalculator(Ptr(new BasicHeurCalculator())));
}
