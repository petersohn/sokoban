#include "HeurCalculator.h"

HeurCalculator::Ptr HeurCalculator::create() {
	return Ptr(new AdvancedHeurCalculator(Ptr(new BasicHeurCalculator())));
}
