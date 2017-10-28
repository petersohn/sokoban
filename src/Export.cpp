#include "Checker/BlockListChecker.hpp"
#include "Checker/CorridorChecker.hpp"
#include "Checker/DistanceChecker.hpp"
#include "Checker/ExtendedCorridorChecker.hpp"
#include "Checker/ExtendedMovableChecker.hpp"
#include "Checker/MovableChecker.hpp"
#include "Checker/VisitedStatesChecker.hpp"

#include "HeurCalculator/BasicHeurCalculator.hpp"
#include "HeurCalculator/AdvancedHeurCalculator.hpp"
#include "HeurCalculator/BlockListHeurCalculator.hpp"
#include "HeurCalculator/DecisionTreeHeurCalculator.hpp"
#include "Status/Status.hpp"

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/export.hpp>

BOOST_CLASS_EXPORT(sokoban::MovableChecker)
BOOST_CLASS_EXPORT(sokoban::ExtendedMovableChecker)
BOOST_CLASS_EXPORT(sokoban::CorridorChecker)
BOOST_CLASS_EXPORT(sokoban::ExtendedCorridorChecker)
BOOST_CLASS_EXPORT(sokoban::BlockListChecker)

BOOST_CLASS_EXPORT(sokoban::DistanceChecker)
BOOST_CLASS_EXPORT(sokoban::VisitedStatesChecker)

BOOST_CLASS_EXPORT(sokoban::BasicHeurCalculator)
BOOST_CLASS_EXPORT(sokoban::AdvancedHeurCalculator)
BOOST_CLASS_EXPORT(sokoban::BlockListHeurCalculator)
BOOST_CLASS_EXPORT(sokoban::DecisionTreeHeurCalculator)

