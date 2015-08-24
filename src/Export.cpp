#include "Checker/BlockListChecker.hpp"
#include "HeurCalculator/BasicHeurCalculator.hpp"
#include "HeurCalculator/AdvancedHeurCalculator.hpp"
#include "HeurCalculator/BlockListHeurCalculator.hpp"
#include "HeurCalculator/DecisionTreeHeurCalculator.hpp"
#include "Status/Status.hpp"

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/export.hpp>

BOOST_CLASS_EXPORT(BlockListChecker)

BOOST_CLASS_EXPORT(BasicHeurCalculator)
BOOST_CLASS_EXPORT(AdvancedHeurCalculator)
BOOST_CLASS_EXPORT(BlockListHeurCalculator)
BOOST_CLASS_EXPORT(DecisionTreeHeurCalculator)

